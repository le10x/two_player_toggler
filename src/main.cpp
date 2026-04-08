#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    bool init() {
        if (!PauseLayer::init()) return false;

        // Intentamos cargar tu imagen. Si no existe, usamos el botón de Play por defecto.
        auto buttonSprite = CCSprite::createWithSpriteFrameName("my_button.png");
        if (!buttonSprite) {
            buttonSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
        }
        
        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onMyButtonClick)
        );

        // Buscamos el menú izquierdo por su ID de Geode
        if (auto leftMenu = this->getChildByID("left-button-menu")) {
            myButton->setID("hello-world-button"_spr);
            leftMenu->addChild(myButton);
            leftMenu->updateLayout(); 
        }

        return true;
    }

    void onMyButtonClick(CCObject* sender) {
        FLAlertLayer::create(
            "Prueba",           
            "Hello World",      
            "OK"                
        )->show();
    }
};
