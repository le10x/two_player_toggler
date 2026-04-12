#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

bool g_originalTwoPlayerState = false;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;
        g_originalTwoPlayerState = m_levelSettings->m_twoPlayerMode;
        return true;
    }

    // Usamos dispatchClick porque nos da la posición (x, y) del toque/mouse
    void dispatchClick(bool push, int button, bool isPlayer1, CCPoint pos) {
        bool split = Mod::get()->getSettingValue<bool>("split-controls");
        bool invert = Mod::get()->getSettingValue<bool>("invert-split");

        if (split && m_levelSettings->m_twoPlayerMode) {
            // Obtenemos el ancho de la pantalla
            float winWidth = CCDirector::sharedDirector()->getWinSize().width;
            
            // Determinamos si el toque fue en la mitad derecha (true) o izquierda (false)
            bool isRightSide = pos.x > (winWidth / 2);

            // Por defecto: Izquierda = P1, Derecha = P2
            // Si invert es true: Izquierda = P2, Derecha = P1
            bool targetIsPlayer1 = !isRightSide;
            if (invert) targetIsPlayer1 = isRightSide;

            // Llamamos a la función original forzando al jugador detectado
            PlayLayer::dispatchClick(push, button, targetIsPlayer1, pos);
        } else {
            // Si la opción está apagada, comportamiento normal
            PlayLayer::dispatchClick(push, button, isPlayer1, pos);
        }
    }

    // Mantén tus funciones de resetLevel y onQuit aquí...
};
    
