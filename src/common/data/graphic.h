/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**     Universidad Nacional de Rosario - Argentina.
**  Contact: PowerDEVS Information (kofman@fceia.unr.edu.ar, fbergero@fceia.unr.edu.ar)
**
**  This file is part of PowerDEVS.
**
**  PowerDEVS is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  PowerDEVS is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with PowerDEVS.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <string>

using namespace std;

class Graphic {
public:
	typedef enum {
		ATOMIC_GRAPHIC,
		COUPLED_GRAPHIC,
		NO_GRAPHIC
	} graphicType;
  	Graphic();
	enum Direction { RIGHT, DOWN, LEFT, UP};
	bool isNoGraphic() { return _type == NO_GRAPHIC; };
	int x() const { return _x; };
	int y() const { return _y; };
	int width() const { return _width; };
	int height() const { return _height; };
	int color() const { return _color; };
	string icon() const { return _icon; };
	Direction direction() const { return _direction; };
	string directionWord() const;
	void setX(int x) { _x=x;};
	void setY(int y) { _y=y;};
	void setWidth(int width) { _width=width; };
	void setHeight(int height) { _height=height;};
	void setDirection(Direction direction) { _direction=direction;};
	void setColor(int color) { _color=color;};
	void setIcon(string icon) { _icon=icon;};
  	virtual ~Graphic() {};
  	friend ostream &operator<<(ostream &out, const Graphic);

private:
	int _x;
	int _y;
	int _width;
	int _height;
	Direction _direction;
	int _color;
	string _icon;
protected:
	graphicType _type;
};

class NoGraphic: public Graphic
{
public:
	NoGraphic() { _type = NO_GRAPHIC; };
};

class CoupledGraphic: public Graphic
{
public:
	CoupledGraphic() { _type = COUPLED_GRAPHIC; };
};

class AtomicGraphic: public Graphic
{
public:
	AtomicGraphic() { _type = ATOMIC_GRAPHIC; };
};
#endif
