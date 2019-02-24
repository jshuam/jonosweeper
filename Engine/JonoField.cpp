#include "JonoField.h"

JonoField::JonoField( int width, int height )
{
	assert( width >= 0 && width <= Graphics::ScreenWidth );
	assert( height >= 0 && height <= Graphics::ScreenHeight );
	this->width = width;
	this->height = height;
	this->field = new Tile[width * height];
	this->mines = ( width * height ) * minePercentage;
	this->fieldOffsetX = ( Graphics::ScreenWidth / 2 ) - ( ( width * SpriteCodex::tileSize ) / 2 );
	this->fieldOffsetY = ( Graphics::ScreenHeight / 2 ) - ( ( height * SpriteCodex::tileSize ) / 2 );
}

JonoField::~JonoField()
{
	delete[] field;
}

void JonoField::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( GetRect(), SpriteCodex::baseColor );

	for( Vei2 fieldPos = { 0, 0 }; fieldPos.y < height; fieldPos.y++ )
	{
		for( fieldPos.x = 0; fieldPos.x < width; fieldPos.x++ )
		{
			Vei2 drawPos = fieldPos * SpriteCodex::tileSize;
			drawPos.x += fieldOffsetX;
			drawPos.y += fieldOffsetY;
			GetTile( fieldPos ).Draw( drawPos, gfx, gameOver );
		}
	}

	if( gameOver )
	{
		SpriteCodex::DrawRestartText( { 0, fieldOffsetY }, gfx );
		SpriteCodex::DrawGameOver( { width * SpriteCodex::tileSize + fieldOffsetX, fieldOffsetY }, gfx );
	}
}

void JonoField::RevealClickedTile( Vei2& mousePos )
{
	int boundWidth = width * SpriteCodex::tileSize + fieldOffsetX;
	int boundHeight = height * SpriteCodex::tileSize + fieldOffsetY;
	assert( mousePos.x >= fieldOffsetX &&  mousePos.x <= boundWidth && mousePos.y >= fieldOffsetY && mousePos.y <= boundHeight );
	Vei2 mouseFieldPos = ConvertToFieldPos( mousePos );

	if( firstClick )
	{
		InitJonoField( mouseFieldPos );
		firstClick = false;
	}

	if( !GetTile( mouseFieldPos ).IsRevealed() && !GetTile( mouseFieldPos ).IsFlagged() )
	{
		if( GetTile( mouseFieldPos ).HasJono() )
		{
			GetTile( mouseFieldPos ).Reveal();
			gameOver = true;
			gameOverSound.Play(1.0f, 0.25f);
		}
		else
		{
			RevealNearbyJonos( mouseFieldPos );
		}
	}
}

void JonoField::FlagClickedTile( Vei2& mousePos )
{
	int boundWidth = width * SpriteCodex::tileSize + fieldOffsetX;
	int boundHeight = height * SpriteCodex::tileSize + fieldOffsetY;
	assert( mousePos.x >= fieldOffsetX && mousePos.x <= boundWidth && mousePos.y >= fieldOffsetY && mousePos.y <= boundHeight );
	Vei2 mouseFieldPos = ConvertToFieldPos( mousePos );
	if( !GetTile( mouseFieldPos ).IsRevealed() )
	{
		GetTile( mouseFieldPos ).ToggleFlag();
	}
}

void JonoField::ResetGame()
{
	gameOverSound.StopAll();
	gameOver = false;
	firstClick = true;
	delete[] field;
	this->field = new Tile[width * height];
}

void JonoField::Tile::Draw( Vei2 pos, Graphics& gfx, bool gameOver ) const
{
	if( !gameOver )
	{
		switch( state )
		{
			case State::Hidden:
				SpriteCodex::DrawTileButton( pos, gfx );
				break;
			case State::Revealed:
				SpriteCodex::DrawTileN( pos, gfx, nearbyJonos );
				break;
			case State::Flagged:
				SpriteCodex::DrawTileButton( pos, gfx );
				SpriteCodex::DrawTileFlag( pos, gfx );
				break;
			default:
				break;
		}
	}
	else
	{
		switch( state )
		{
			case State::Hidden:
				if( HasJono() )
				{
					SpriteCodex::DrawTileJono( pos, gfx );
				}
				else
				{
					SpriteCodex::DrawTile0( pos, gfx );
				}
				break;
			case State::Revealed:
				if( !HasJono() )
				{
					SpriteCodex::DrawTile0( pos, gfx );
				}
				else
				{
					SpriteCodex::DrawTileJonoRed( pos, gfx );
				}
				break;
			case State::Flagged:
				if( !HasJono() )
				{
					SpriteCodex::DrawTileFlag( pos, gfx );
					SpriteCodex::DrawTileCross( pos, gfx );
				}
				else
				{
					SpriteCodex::DrawTileJono( pos, gfx );
					SpriteCodex::DrawTileFlag( pos, gfx );
				}
				break;
			default:
				break;
		}
	}
}

void JonoField::Tile::SpawnJono()
{
	assert( !hasJono );
	hasJono = true;
}

void JonoField::Tile::Reveal()
{
	assert( state == State::Hidden );
	state = State::Revealed;
}

void JonoField::Tile::ToggleFlag()
{
	assert( !IsRevealed() );
	if( state == State::Flagged )
	{
		state = State::Hidden;
	}
	else
	{
		state = State::Flagged;
	}
}

bool JonoField::Tile::IsRevealed()
{
	return state == State::Revealed;
}

bool JonoField::Tile::IsFlagged()
{
	return state == State::Flagged;
}

bool JonoField::Tile::HasJono() const
{
	return hasJono;
}

void JonoField::Tile::SetNearbyJonos( int numJonos )
{
	assert( numJonos >= 0 && numJonos <= 8 );
	nearbyJonos = numJonos;
}

int JonoField::Tile::GetNearbyJonos()
{
	return nearbyJonos;
}

JonoField::Tile& JonoField::GetTile( Vei2 pos )
{
	return field[pos.x + pos.y * width];
}

const JonoField::Tile& JonoField::GetTile( Vei2 pos ) const
{
	return field[pos.x + pos.y * width];
}

RectI JonoField::GetRect() const
{
	return RectI( 0 + fieldOffsetX, width * SpriteCodex::tileSize + fieldOffsetX, 0 + fieldOffsetY, height * SpriteCodex::tileSize + fieldOffsetY );
}

bool JonoField::IsGameOver() const
{
	return gameOver;
}

const Vei2& JonoField::ConvertToFieldPos( Vei2& pos ) const
{
	pos.x -= fieldOffsetX;
	pos.y -= fieldOffsetY;
	return pos / SpriteCodex::tileSize;
}

int JonoField::CountNearbyJonos( const Vei2& fieldPos )
{
	int numJonos = 0;
	int startX = std::max( 0, fieldPos.x - 1 );
	int startY = std::max( 0, fieldPos.y - 1 );
	int endX = std::min( width - 1, fieldPos.x + 1 );
	int endY = std::min( height - 1, fieldPos.y + 1 );

	for( Vei2 fieldPos = { startX, startY }; fieldPos.y <= endY; fieldPos.y++ )
	{
		for( fieldPos.x = startX; fieldPos.x <= endX; fieldPos.x++ )
		{
			if( GetTile( fieldPos ).HasJono() )
			{
				numJonos++;
			}
		}
	}

	return numJonos;
}

void JonoField::RevealNearbyJonos( const Vei2& fieldPos )
{
	int startX = std::max( 0, fieldPos.x - 1 );
	int startY = std::max( 0, fieldPos.y - 1 );
	int endX = std::min( width - 1, fieldPos.x + 1 );
	int endY = std::min( height - 1, fieldPos.y + 1 );

	if( !GetTile( fieldPos ).IsFlagged() && GetTile( fieldPos ).GetNearbyJonos() == 0 )
	{
		GetTile( fieldPos ).Reveal();
		for( Vei2 fieldPos = { startX, startY }; fieldPos.y <= endY; fieldPos.y++ )
		{
			for( fieldPos.x = startX; fieldPos.x <= endX; fieldPos.x++ )
			{
				if( !GetTile( fieldPos ).IsRevealed() )
				{
					RevealNearbyJonos( fieldPos );
				}
			}
		}
	}
	else if( !GetTile( fieldPos ).IsFlagged() )
	{
		GetTile( fieldPos ).Reveal();
	}
}

void JonoField::InitJonoField( const Vei2& mouseFieldPos )
{
	int startX = std::max( 0, mouseFieldPos.x - 1 );
	int startY = std::max( 0, mouseFieldPos.y - 1 );
	int endX = std::min( width - 1, mouseFieldPos.x + 1 );
	int endY = std::min( height - 1, mouseFieldPos.y + 1 );

	std::random_device rd;
	std::mt19937 mt( rd() );
	std::uniform_int_distribution<int> xDist( 0, width - 1 );
	std::uniform_int_distribution<int> yDist( 0, height - 1 );

	for( int i = 0; i < mines; i++ )
	{
		Vei2 pos;
		do
		{
			pos = Vei2( xDist( mt ), yDist( mt ) );
		}
		while( GetTile( pos ).HasJono() ||
			   pos.x >= startX && pos.x <= endX &&
			   pos.y >= startY && pos.y <= endY );

		GetTile( pos ).SpawnJono();
	}

	for( Vei2 fieldPos = { 0, 0 }; fieldPos.y < height; fieldPos.y++ )
	{
		for( fieldPos.x = 0; fieldPos.x < width; fieldPos.x++ )
		{
			GetTile( fieldPos ).SetNearbyJonos( CountNearbyJonos( fieldPos ) );
		}
	}
}
