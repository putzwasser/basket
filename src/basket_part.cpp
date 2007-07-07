/***************************************************************************
 *   Copyright (C) 2003 by Petri Damsten                                   *
 *   petri.damsten@iki.fi                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "basket_part.h"

#include <kinstance.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <klocale.h>
#include <bnpview.h>
#include <aboutdata.h>
#include <kparts/genericfactory.h>
#include <kparts/statusbarextension.h>
#include "basketstatusbar.h"

typedef KParts::GenericFactory< BasketPart > BasketFactory;
K_EXPORT_COMPONENT_FACTORY( libbasketpart, BasketFactory )

BasketPart::BasketPart( QWidget *parentWidget, const char *,
						QObject *parent, const char *name, const QStringList & )
	: KParts::ReadWritePart(parent, name)
{
  // we need an instance
	setInstance( BasketFactory::instance() );

	BasketStatusBar* bar = new BasketStatusBar(new KParts::StatusBarExtension(this));
  // this should be your custom internal widget
	m_view = new BNPView(parentWidget, "BNPViewPart", this, actionCollection(), bar);
	connect(m_view, SIGNAL(setWindowCaption(const QString &)), this, SLOT(setCaption(const QString &)));
	connect(m_view, SIGNAL(showPart()), this, SIGNAL(showPart()));
	m_view->setFocusPolicy(QWidget::ClickFocus);

  // notify the part that this is our internal widget
	setWidget(m_view);

  // set our XML-UI resource file
	setXMLFile("basket_part.rc");

  // we are read-write by default
	setReadWrite(true);

  // we are not modified since we haven't done anything yet
	setModified(false);
}

BasketPart::~BasketPart()
{}

void BasketPart::setReadWrite(bool rw)
{
  // TODO: notify your internal widget of the read-write state
	ReadWritePart::setReadWrite(rw);
}

void BasketPart::setModified(bool modified)
{
  // in any event, we want our parent to do it's thing
	ReadWritePart::setModified(modified);
}

bool BasketPart::openFile()
{
  // TODO
	return false;
}

bool BasketPart::saveFile()
{
  //TODO
	return false;
}

KAboutData *BasketPart::createAboutData()
{
	return new AboutData();
}

void BasketPart::setCaption(const QString &caption)
{
	emit setWindowCaption(caption);
}

#include "basket_part.moc"
