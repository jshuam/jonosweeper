#pragma once

#include "Graphics.h"
#include "SpriteCodex.h"
#include "Vei2.h"

class JonoField
{
public:
	JonoField(int mines);

	void Draw(Graphics& gfx) const;

private:
	class Tile
	{
	public:
		Tile();
		void Draw(Graphics& gfx) const;
		bool HasJono() const;

	private:
		enum class State
		{
			Hidden,
			Revealed,
			Flagged
		};

	private:
		bool hasJono = false;
		State state = State::Hidden;
		Vei2 pos;
	};

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	int mines;

	static Tile field[width * height];
};