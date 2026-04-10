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
        FLAlertLayer::create(
            "2 Player Toggler",
            "Use this switch to enable or disable <cl>2 Player Mode</c>.\n\n"
            "<cy>Note:</cy> Controls will be split on both sides of the screen.",
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
        auto leftMenu = this->getChildByID("left-button-menu");

        if (leftMenu) {
            auto container = CCNode::create();
            container->setContentSize({45, 60});
            container->setID("two-player-group"_spr);

            auto toggler = CCMenuItemToggler::createWithStandardSprites(
                this,
                menu_selector(MyPauseLayer::onToggleTwoPlayer),
                0.9f 
            );
            toggler->toggle(levelSettings->m_twoPlayerMode);
            toggler->setPosition({22, 22});

            auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            infoSprite->setScale(0.65f);
            
            auto infoBtn = CCMenuItemSpriteExtra::create(
                infoSprite,
                this,
                menu_selector(MyPauseLayer::onInfo)
            );
            infoBtn->setPosition({38, 42}); 

            auto innerMenu = CCMenu::create();
            innerMenu->setPosition({0, 0});
            innerMenu->addChild(toggler);
            innerMenu->addChild(infoBtn);

            container->addChild(innerMenu);
            leftMenu->addChild(container);
            leftMenu->updateLayout();
        }

        return true;
    }
};
