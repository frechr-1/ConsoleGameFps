#define NOMINMAX
#pragma once
#include "SparseArray.hpp"
#include "ConsoleManager.hpp"
#include "IKeyState.hpp"
#include "InputManager.hpp"
#include "MovementController.hpp"
#include "SymbolComponent.hpp"
#include "TranslateComponent.hpp"
#include "BoxCollider.hpp"
#include "AIController.hpp"
#include "PhysicsController.hpp"
#include "EntityManager.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <execution>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <cmath> 
using Entity = size_t;


// TODO: Extract to Game.hpp in the "run" method.
std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();
double accumulatedTime = 0.0;
const double timeStep = 1.0 / 100.0;

// TODO: SystemManager<TranslateSystem>() perhaps?
void Translate(Entity entity, SparseArray<Translation> & translations,
			   unsigned int maxWidth, unsigned int maxHeight)
{}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{

	EntityManager<1000> em;
	
	// TODO: Game class with initializers
	const unsigned int W = 98;
	const unsigned int H = 77;
	// Refactor to "console renderer"? 
	ConsoleManager<W, H> cm(L' ');
	cm.attachConsole();

	// Event system and use KeyBind component instead?
	IKeyState * ks = new KeyState();

	Entity player = em.createEntity();
	Entity other1 = em.createEntity();
	Entity other2 = em.createEntity();
	Entity debug =  em.createEntity();
	Entity debug1 = em.createEntity();
	Entity debug2 = em.createEntity();
	Entity debug3 = em.createEntity();
	Entity debug4 = em.createEntity();

	// TODO: Archetypes for differentiating between players and enemies? Tags?
	std::array<Entity *, 2> others{ &other1, &other2 };

	// TODO: Hide implementation details, only componentmanager should ever see the words "sparse array"
	SparseArray<Translation> translations;				    

	Translation debugT{
		1,	 1 
	};
	Translation debugT1{
		2,	 1 
	};
	Translation debugT2{
		3,	 1 
	};
	Translation debugT3{
		4,	 1 
	};		
	Translation debugT4{
		5,	 1 
	};
	Translation fastButInaccurate{
		  W-1, H-1 , // X Y
		  0, 0  , // DX DY
		1.5, 0.025, // DMAX INC
		  1, 10 , // XOLD YOLD
		false	  // CHANGED
	};
	SparseArray<Symbol> symbols;
	translations.add(player, {
		20, 20,
		0, 0,
		1.0, 0.1,
		1, 10,
		false });
	translations.add(other1, fastButInaccurate);
	translations.add(other2, fastButInaccurate);  
	translations.add(debug, debugT);
	translations.add(debug1, debugT1);
	translations.add(debug2, debugT2);
	translations.add(debug3, debugT3);
	translations.add(debug4, debugT4);


	symbols.add(player, { L'0' });
	symbols.add(other1, { L'*' });
	symbols.add(other2, { L'*' });

	symbols.add(debug, { L'-' });
	symbols.add(debug1, { L'-' });
	symbols.add(debug2, { L'-' });
	symbols.add(debug3, { L'-' });
	symbols.add(debug4, { L'-' });

	InputManager im(*ks);
	MovementController mc(&translations, player);
	MovementController ec1(&translations, other1);
	MovementController ec2(&translations, other2); 

	im.addBinding('D', [&mc]() { mc.accelerateRight(); });
	im.addBinding('A', [&mc]() { mc.accelerateLeft(); });
	im.addBinding('W', [&mc]() { mc.accelerateUp(); });
	im.addBinding('S', [&mc]() { mc.accelerateDown(); });
	im.addBinding('Q', [&ks]() { delete ks; });

	while ( true ) {

		// TODO: Extract to Game.hpp
		auto now = std::chrono::steady_clock::now();
		double deltaTime = std::chrono::duration<double>(now - lastUpdateTime).count();
		lastUpdateTime = now;
		accumulatedTime += deltaTime;

		// TODO: Utility function in Game.hpp, debugging
		std::string cs = std::to_string(translations.get(player)->x);
		symbols.set(debug, static_cast<Symbol>(cs.at(0)));
		symbols.set(debug1, static_cast<Symbol>(cs.at(1)));
		symbols.set(debug2, static_cast<Symbol>(cs.at(2)));
		symbols.set(debug3, static_cast<Symbol>(cs.at(3)));
		symbols.set(debug4, static_cast<Symbol>(cs.at(4)));

		// Check if accumulated time has reached or exceeded the fixed timestep, if yes than one tick has passed
		if ( accumulatedTime >= timeStep ) {
			// TODO: Encapsulate in SystemManager's update method.
			im.handleInput();
			mc.update();
			AIController::seekAndDestroy(&ec1, &player, &translations);
			AIController::seekAndDestroy(&ec2, &player, &translations);
			ec1.update();
			ec2.update();
			//BoxCollider::checkCollisions(&translations);	  // Simple collider, just so characters wont overlap
			//PhysicsController::updatePhysics(&translations);
			
			// TODO: Create a bounds check system, or incorporate into console renderer?
			for ( int i = 0; i < em.size(); i++ ) {
				if ( !em.isActive(i) )
					continue;
				Translation * t = translations.get(i);
				if ( t ) {
					t->x = std::min(std::max(0.0f, t->x + t->dx), static_cast<float>(W) - 1.0f);
					t->y = std::min(std::max(0.0f, t->y + t->dy), static_cast<float>(H) - 1.0f);
				}
			}
			accumulatedTime -= timeStep;
		}

		// TODO: Extract to game.hpp
		cm.UpdateBuffer(translations, symbols);	
		cm.Render();
	}
	return 0;
}
