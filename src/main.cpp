#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

bool g_originalTwoPlayerState = false;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;
        g_originalTwoPlayerState = m_levelSettings->m_twoPlayerMode;
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        if (Mod::get()->getSettingValue<bool>("reset-on-death")) {
            m_levelSettings->m_twoPlayerMode = g_originalTwoPlayerState;
        }
    }

    void onQuit() {
        m_levelSettings->m_twoPlayerMode = g_originalTwoPlayerState;
        PlayLayer::onQuit();
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void onInfo(CCObject* sender) {
        // Corrección definitiva: <cy>Note:</c> para que aparezca en amarillo
        FLAlertLayer::create(
            "2 Player Toggler",
            "Use this switch to enable or disable <cl>2 Player Mode</c>.\n\n"
            "<cy>Note:</c> Controls will be split on both sides of the screen.",
            "OK"
        )->show();
    }

    void onToggleTwoPlayer(CCObject* sender) {
        auto levelSettings = PlayLayer::get()->m_levelSettings;
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        levelSettings->m_twoPlayerMode = !toggler->isToggled();
    }

    bool init(bool unfocused) {
        if (!PauseLayer::init(unfocused)) return false;

        auto levelSettings = PlayLayer::get()->m_levelSettings;

        // 1. Toggler (Check) - Reducido a 0.65f (25% más pequeño que 0.9f)
        auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyPauseLayer::onToggleTwoPlayer),
            0.65f 
        );
        toggler->toggle(levelSettings->m_twoPlayerMode);
        toggler->setPosition({0, 0});

        // 2. Botón de Info (Reducido a 0.6f y movido 1mm a la derecha: x=39)
        auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        auto infoBtn = CCMenuItemSpriteExtra::create(
            infoSprite,
            this,
            menu_selector(MyPauseLayer::onInfo)
        );
        infoBtn->setScale(0.6f);
        infoBtn->setPosition({39, 25}); // x=39 movido un milímetro a la derecha

        // 3. Texto "2-Player Mode" - Reducido a 0.35f
        auto label = CCLabelBMFont::create("2-Player Mode", "bigFont.fnt");
        label->setScale(0.35f);
        label->setAnchorPoint({0, 0.5});
        label->setPosition({20, 0}); // Pegado a la derecha del check

        // 4. Menú contenedor en la esquina inferior izquierda
        auto menu = CCMenu::create();
        menu->addChild(toggler);
        menu->addChild(infoBtn);
        menu->addChild(label);
        
        // Coordenadas fijas para la esquina
        menu->setPosition({30, 25});
        menu->setID("two-player-menu"_spr);

        this->addChild(menu);

        return true;
    }
};
