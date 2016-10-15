#pragma once

void keyInit( Window* win );
void keyEnd();

void keyUpdate();

bool keyIsPress( ButtonId id );
bool keyIsTrigger( ButtonId id );
