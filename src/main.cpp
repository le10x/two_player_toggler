#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    bool init() {
        if (!PauseLayer::init()) return false;

        auto menu = this->getChildByID("left-button-menu");
        if (!menu) return true;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png");
        auto btn = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(MyPauseLayer::onMyButton)
        );

        btn->setID("hello-world-btn"_spr);
        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }

    void onMyButton(CCObject*) {
        FLAlertLayer::create("Geode", "Hello World", "OK")->show();
    }
};
