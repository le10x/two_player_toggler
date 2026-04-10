#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    bool init(bool unfocused) {
        if (!PauseLayer::init(unfocused)) return false;

        // 1. Revisamos si la configuración está activa
        bool isEnabled = Mod::get()->getSettingValue<bool>("activate");

        if (isEnabled) {
            // 2. Buscamos el botón de comentarios y los menús laterales
            auto commentBtn = this->getChildByIDRecursive("comments-button");
            auto leftMenu = this->getChildByID("left-button-menu");
            auto rightMenu = this->getChildByID("right-button-menu");

            if (commentBtn && leftMenu && rightMenu) {
                // 3. Lo quitamos de la izquierda y lo pasamos a la derecha
                commentBtn->removeFromParentAndCleanup(false);
                rightMenu->addChild(commentBtn);
                
                // 4. Actualizamos ambos layouts para que no se amontonen
                leftMenu->updateLayout();
                rightMenu->updateLayout();
            }
        }

        return true;
    }
};
