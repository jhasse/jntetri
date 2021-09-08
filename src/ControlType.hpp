#pragma once

enum class ControlType {
	Drop,
	Left,
	Right,
	Down,
	Rotate,
	RotateCounter,
	Null,     // dummy package used in network games and replays
	AddJunk,  // only for multiplayer
	LastValue // must always be the last (see bitset in Control class)
};
