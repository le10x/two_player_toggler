#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    // Definimos la función como parte del "wrapper"
    void onMyButtonClick(CCObject* sender) {
        FLAlertLayer::create(
            "Prueba",           
            "Hello World",      
            "OK"                
        )->show();
    }

    bool init() {
        if (!PauseLayer::init()) return false;

        // Intentamos cargar tu imagen personalizada
        auto buttonSprite = CCSprite::createWithSpriteFrameName("my_button.png");
        
        // Si la imagen no carga (porque no existe o está mal el nombre), 
        // usamos una del juego para que no crashee
        if (!buttonSprite) {
            buttonSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
            buttonSprite->setScale(0.5f); // El botón de play es muy grande
        }
        
        auto myButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onMyButtonClick) // Ahora sí lo encontrará
        );
    
        // Buscamos el menú de la izquierda del menú de pausa
        if (auto leftMenu = this->getChildByID("left-button-menu")) {
            myButton->setID("test-button"_spr);
            leftMenu->addChild(myButton);
            leftMenu->updateLayout(); 
        }

        return true;
    }
};
