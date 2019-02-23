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
		void Draw( Vei2 pos, Graphics& gfx ) const;
		void SpawnJono();
		void Reveal();
		bool IsRevealed();
		bool HasJono() const;

	private:
		bool hasJono = false;
		State state = State::Hidden;
	};

public:
	JonoField( int mines );
	void Draw( Graphics& gfx ) const;
	void RevealClickedTile( const Vei2& mousePos );

private:
	Tile& GetTile( Vei2 pos );
	const Tile& GetTile( Vei2 pos ) const;
	RectI GetRect() const;
	const Vei2& ConvertToFieldPos( const Vei2& pos ) const;

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile field[width * height];
};