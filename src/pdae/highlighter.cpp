/***************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**       	       Universidad Nacional de Rosario - Argentina.
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
 
#include <QtGui>
    
#include "highlighter.h"
    
//! [0]
 Highlighter::Highlighter(QTextDocument * parent) :
QSyntaxHighlighter(parent)
    
{
	HighlightingRule rule;
	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" 
	    <<"\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b" 
	    <<"\\bfriend\\b" << "\\binline\\b" << "\\bint\\b" 
	    <<"\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b" 
	    <<"\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" 
	    <<"\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b" 
	    <<"\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b" 
	    <<"\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b" 
	    <<"\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b" 
	    <<"\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b" << "\\bcase\\b" 
	    <<"\\bfor\\b" << "\\bif\\b" << "\\belse\\b" << "\\bnew\\b" 
	    <<"\\breturn\\b" << "\\bwhile\\b" << "\\bvoid\\b";
	foreach(QString pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
		
//! [0] //! [1]
	} 
//! [1]
	    
//! [2]
	    classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);
	
//! [2]
	    
//! [3]
	    singleLineCommentFormat.setForeground(Qt::gray);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);
	multiLineCommentFormat.setForeground(Qt::gray);
	
//! [3]
	    
//! [4]
	    quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);
	
//! [4]
	    
//! [5]
	    functionFormat.setFontItalic(true);
	functionFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);
	
//! [5]
	    
//! [6]
	    commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
} 

//! [6]
    
//! [7]
void Highlighter::highlightBlock(const QString & text) 
{
	foreach(HighlightingRule rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expression, index + length);
	} } 
//! [7] //! [8]
	    setCurrentBlockState(0);
	
//! [8]
	    
//! [9]
	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf(commentStartExpression);
	
//! [9] //! [10]
	    while (startIndex >= 0) {
		
//! [10] //! [11]
		int endIndex = text.indexOf(commentEndExpression, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex 
			    +commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex =
		    text.indexOf(commentStartExpression,
				 startIndex + commentLength);
	}
}


//! [11]
