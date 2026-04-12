#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
// Añadimos este para que reconozca la función base
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

bool g_originalTwoPlayerState = false;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;
        g_originalTwoPlayerState = m_levelSettings->m_twoPlayerMode;
        return true;
    }

    void dispatchClick(bool push, int button, bool isPlayer1, CCPoint pos) {
        bool split = Mod::get()->getSettingValue<bool>("split-controls");
        bool invert = Mod::get()->getSettingValue<bool>("invert-split");

        if (split && m_levelSettings->m_twoPlayerMode) {
            float winWidth = CCDirector::sharedDirector()->getWinSize().width;
            bool isRightSide = pos.x > (winWidth / 2);

            bool targetIsPlayer1 = !isRightSide;
            if (invert) targetIsPlayer1 = isRightSide;

            // Cambiamos PlayLayer:: por GJBaseGameLayer::
            GJBaseGameLayer::dispatchClick(push, button, targetIsPlayer1, pos);
        } else {
            GJBaseGameLayer::dispatchClick(push, button, isPlayer1, pos);
        }
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

// ... (El resto de tu MyPauseLayer se mantiene igual)
