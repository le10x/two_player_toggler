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
    void customSetup() {
        PauseLayer::customSetup();
        
        auto levelSettings = PlayLayer::get()->m_levelSettings;

        auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyPauseLayer::onToggleTwoPlayer),
            0.67f 
        );
        toggler->toggle(levelSettings->m_twoPlayerMode);

        auto label = CCLabelBMFont::create("2-Player Mode", "bigFont.fnt");
        label->setScale(0.3f);
        label->setAnchorPoint({0, 0.5});
        label->setPosition({20, 0});

        auto menu = CCMenu::create();
        menu->addChild(toggler);
        menu->addChild(label);
        
        menu->setPosition({42, 37});
        menu->setID("two-player-menu"_spr);

        this->addChild(menu);
    }

    void onToggleTwoPlayer(CCObject* sender) {
        auto levelSettings = PlayLayer::get()->m_levelSettings;
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        levelSettings->m_twoPlayerMode = !toggler->isToggled();
    }
};
