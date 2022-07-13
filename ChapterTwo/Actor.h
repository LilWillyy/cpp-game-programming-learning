#pragma once
#include <vector>
#include "Math.h"

class Actor 
{
public:
	// Used to track state of actor
	enum State {
		EActive,
		EPaused,
		EDead
	};

	// Constructor/destructor
	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from update
	void Update(float deltaTime);
	// Updates all components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	// Getters/setters
	// ...

	const Vector2& GetPosition() const { return mPosition; };
	void SetPosition(const Vector2& pos) { mPosition = pos; };


	float GetScale() const { return mScale; };
	void SetScale(float scale) { mScale = scale; };
	float GetRotation() const { return mRotation; };
	void SetRotation(float rotation) { mRotation = rotation; };

	State GetState() const { return mState; };
	void SetState(State state) { mState = state; };
	
	class Game* GetGame() { return mGame; };
	
	
	// Add/Remove componenets
	void AddComponent(class Component* componenent);
	void RemoveComponent(class Component* component);
private:
	// Actors state
	State mState;

	// Transform
	Vector2 mPosition; // Center position for actor
	float mScale;	   // Uniforms scale for actor (1.0f for 100%)
	float mRotation;   // Rotation angle (in radians)

	std::vector<class Component*> mComponents;
	class Game* mGame;
};