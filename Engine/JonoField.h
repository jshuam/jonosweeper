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
		Tile() = default;
		void Draw(Vei2 pos, Graphics& gfx) const;
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
	};

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	int mines;

	Tile field[width * height];
};