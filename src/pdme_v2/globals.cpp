/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**     Universidad Nacional de Rosario - Argentina.
**  Contact: PowerDEVS Information (kofman@fceia.unr.edu.ar, fbergero@fceia.unr.edu.ar)
**
**  This file is part of PowerDEVS.
**
**  PowerDEVS is free software: you can redisibute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  PowerDEVS is disibuted in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with PowerDEVS.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <iostream>
#include <fstream>

#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include <globals.h>
#include <parser.h>

QVariant getSetting(QString key)
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/powerdevs.ini", QSettings::IniFormat);
	return settings.value(key);
}

void setSetting(QString key,QVariant value)
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/powerdevs.ini", QSettings::IniFormat);
	settings.setValue(key,value);
}

void removeSetting(QString key)
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/powerdevs.ini", QSettings::IniFormat);
	settings.remove("menu/"+key);
}


using namespace std;
Coupled *parseFile(QString fileName)
{
    return parseModel(fileName);
}

QString getImage(QString fileName)
{
  if (fileName.startsWith("\%"))
  {
    fileName = getSetting("Path/libPath").toString() + fileName.replace('%','/').toLower();
  }
#ifdef Q_OS_LINUX
  return fileName.replace('\\','/');
#else
  return fileName.replace('/','\\');
#endif

}

QColor intToColor(int c)
{
	if (c<=0 || c>15) c=15;
	QList < QColor > colors;
	colors  << Qt::black 
          << Qt::darkBlue 
          << Qt::darkGreen 
          << Qt::darkCyan 
          << Qt::darkRed 
          << Qt::darkMagenta 
          << Qt::darkYellow 
          << Qt::gray 
          << Qt::darkGray 
          << Qt::blue 
          << Qt::green 
          << Qt::cyan 
          << Qt::red 
          << Qt::lightGray 
          << Qt::white;
	return colors.at(c-1);
}

int colorToInt(QColor c)
{
	QList < QColor > colors;
  colors  << Qt::black 
          << Qt::darkBlue 
          << Qt::darkGreen 
          << Qt::darkCyan 
          << Qt::darkRed 
          << Qt::darkMagenta 
          << Qt::darkYellow 
          << Qt::gray 
          << Qt::darkGray 
          << Qt::blue 
          << Qt::green 
          << Qt::cyan 
          << Qt::red 
          << Qt::lightGray 
          << Qt::white;
	return colors.indexOf(c);
}

QString colorToName(int c)
{
	if (c<=0 || c>15) c=15;
	QStringList names;
	names << ("black") 
        << ("darkBlue") 
        << ("darkGreen") 
        << ("darkCyan") 
        << ("darkRed") 
			  << ("darkMagenta") 
        << ("darkYellow") 
        << ("gray") 
        << ("darkGray")  
			  << ("blue") 
        << ("green") 
        << ("cyan") 
        << ("red") 
			  << ("lightGray")
        << ("white") ;
  return names.at(c-1);
}
