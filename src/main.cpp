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
        // Corrección del tag </cy> a </c> para que "Note:" sea amarillo
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
        auto winSize = CCDirector::get()->getWinSize();

        // 1. Toggler (Check) - Escala 0.9f
        auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyPauseLayer::onToggleTwoPlayer),
            0.9f 
        );
        toggler->toggle(levelSettings->m_twoPlayerMode);

        // 2. Botón de Info (Más grande: 0.8f)
        auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        auto infoBtn = CCMenuItemSpriteExtra::create(
            infoSprite,
            this,
            menu_selector(MyPauseLayer::onInfo)
        );
        infoBtn->setScale(0.8f);
        infoBtn->setPosition({15, 25}); // Arriba del check

        // 3. Texto "2-Player Mode" a la derecha
        auto label = CCLabelBMFont::create("2-Player Mode", "bigFont.fnt");
        label->setScale(0.4f);
        label->setAnchorPoint({0, 0.5}); // Alineado a la izquierda
        label->setPosition({25, 0});

        // 4. Menú contenedor posicionado en la esquina inferior izquierda
        auto menu = CCMenu::create();
        menu->addChild(toggler);
        menu->addChild(infoBtn);
        menu->addChild(label);
        
        // Posición: Esquina abajo a la izquierda (x:35, y:30 para que no toque los bordes)
        menu->setPosition({35, 30});
        menu->setID("two-player-menu"_spr);

        this->addChild(menu);

        return true;
    }
};
