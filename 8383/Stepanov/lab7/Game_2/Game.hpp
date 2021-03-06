//
//  Game.hpp
//  Game_2
//
//  Created by Владислав  Степанов on 25.03.2020.
//  Copyright © 2020 Владислав  Степанов. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <string>

#include "OutOfField.hpp"
#include "UnknownUnit.hpp"
#include "ErrorSpec.hpp"
#include "WrongMotion.hpp"
#include "WrongGamer.hpp"
#include "ErrorSpawn.hpp"

#include "Base.hpp"
#include "Field.hpp"
#include "Landscape.hpp"
#include "ProxyLandscape.hpp"

#include "Facade.hpp"
#include "CommandHandler.hpp"
#include "SpawnHandler.hpp"
#include "ExitHandler.hpp"
#include "MotionHandler.hpp"
#include "SpecHandler.hpp"
#include "SaveHandler.hpp"
#include "LoadHandler.hpp"
#include "Mediator.hpp"
#include <fstream>
#include "ProxyLog.hpp"
#include "Adapter.hpp"
#include "BaseSnapshot.hpp"
#include "FieldSnapshot.hpp"
#include "SaveFile.hpp"

#include "MainState.hpp"
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()


class Game:IObserver {
    
    Field *field = nullptr;
    Base *first = nullptr;
    Base *second = nullptr;
    int height, width;
    
    CommandHandler *actionHandler = nullptr;
    Mediator mediator;
    int specialFirst = 0;
    int specialSecond = 0;
    
    BaseSnapshot *firstSave = nullptr;
    BaseSnapshot *secondSave = nullptr;
    FieldSnapshot *fieldSave = nullptr;
    
    MainState state;
    int winner = 0;
    bool isProt = true;
    int size;
    
public:
    
    Game(bool fastGame, int size);
    ~Game();
    void startGame();
    bool action(bool &endGame, bool &saveGame, bool &loadGame);
    
    
    void Update(Unit *unit) override;


};

#endif /* Game_hpp */

