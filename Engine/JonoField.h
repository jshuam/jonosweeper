#pragma once

#include <assert.h>
#include "Graphics.h"
#include <random>
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
	JonoField( int mines );
	void Draw( Graphics& gfx ) const;
	void RevealClickedTile( const Vei2& mousePos );
	void FlagClickedTile( const Vei2& mousePos );
	RectI GetRect() const;
	bool IsGameOver() const;

private:
	Tile& GetTile( Vei2 pos );
	const Tile& GetTile( Vei2 pos ) const;
	const Vei2& ConvertToFieldPos( const Vei2& pos ) const;
	int CountNearbyJonos( const Vei2& fieldPos );
	void RevealNearbyJonos( const Vei2& fieldPos );

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile field[width * height];

	bool gameOver = false;
};