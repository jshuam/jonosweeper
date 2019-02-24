#pragma once

#include <assert.h>
#include "Graphics.h"
#include <random>
#include "Sound.h"
#include "SpriteCodex.h"
#include "Vei2.h"

class JonoField
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Revealed,
			Flagged
		};

	public:
		void Draw( Vei2 pos, Graphics& gfx, bool gameOver ) const;
		void SpawnJono();
		void Reveal();
		void ToggleFlag();
		bool IsRevealed();
		bool IsFlagged();
		void SetNearbyJonos( int numJonos );
		int GetNearbyJonos();
		bool HasJono() const;

	private:
		bool hasJono = false;
		State state = State::Hidden;
		int nearbyJonos = -1;
	};

public:
	JonoField( int width, int height );
	~JonoField();
	void Draw( Graphics& gfx ) const;
	void RevealClickedTile( Vei2& mousePos );
	void FlagClickedTile( Vei2& mousePos );
	void ResetGame();
	RectI GetRect() const;
	bool IsGameOver() const;

private:
	Tile& GetTile( Vei2 pos );
	const Tile& GetTile( Vei2 pos ) const;
	const Vei2& ConvertToFieldPos( Vei2& pos ) const;
	int CountNearbyJonos( const Vei2& fieldPos );
	void RevealNearbyJonos( const Vei2& fieldPos );
	void InitJonoField( const Vei2& mouseFieldPos );

private:
	int width = 0;
	int height = 0;
	int mines = 0;
	int fieldOffsetX = 0;
	int fieldOffsetY = 0;
	Tile *field;
	float minePercentage = 0.25f;
	bool gameOver = false;
	bool firstClick = true;
	Sound gameOverSound = L"gameover.wav";
};