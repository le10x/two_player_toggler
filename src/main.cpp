#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variable global para recordar cómo era el nivel originalmente
bool g_originalTwoPlayerState = false;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;
        // Guardamos el estado original apenas empieza el nivel
        g_originalTwoPlayerState = m_levelSettings->m_twoPlayerMode;
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        // Si la opción de Geode está activa, reseteamos al morir
        if (Mod::get()->getSettingValue<bool>("reset-on-death")) {
            m_levelSettings->m_twoPlayerMode = g_originalTwoPlayerState;
        }
    }

    void onQuit() {
        // Al salir, siempre devolvemos el nivel a su estado original
        m_levelSettings->m_twoPlayerMode = g_originalTwoPlayerState;
        PlayLayer::onQuit();
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void onToggleTwoPlayer(CCObject* sender) {
        auto levelSettings = PlayLayer::get()->m_levelSettings;
        // El toggler cambia de estado automáticamente, solo aplicamos al juego
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        levelSettings->m_twoPlayerMode = !toggler->isToggled();
    }

    bool init(bool unfocused) {
        if (!PauseLayer::init(unfocused)) return false;

        auto levelSettings = PlayLayer::get()->m_levelSettings;

        // Creamos el Toggler (Checkmark verde oficial)
        auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyPauseLayer::onToggleTwoPlayer),
            0.8f // Tamaño
        );
        
        // Lo ponemos en el estado actual del nivel
        toggler->toggle(levelSettings->m_twoPlayerMode);
        toggler->setID("two-player-toggle"_spr);

        // Añadimos un texto pequeño al lado del check
        auto label = CCLabelBMFont::create("2P", "bigFont.fnt");
        label->setScale(0.4f);
        label->setPosition({-20, 15}); // Ajusta según necesites
        toggler->addChild(label);

        if (auto leftMenu = this->getChildByID("left-button-menu")) {
            leftMenu->addChild(toggler);
            leftMenu->updateLayout();
        }

        return true;
    }
};
