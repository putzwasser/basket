/***************************************************************************
 *   Copyright (C) 2005 by Sébastien Laoût                                 *
 *   slaout@linux62.org                                                    *
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

#ifndef TAGEDIT_H
#define TAGEDIT_H

#include <KDE/KDialog>

#include <QtCore/QList>
#include <QtGui/QItemDelegate>
#include <QtGui/QTreeWidgetItem>

#include "tag.h"

class QCheckBox;
class QFontComboBox;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class KLineEdit;
class QTreeWidget;

class QKeyEvent;
class QMouseEvent;

class KIconButton;
class KPushButton;
class KShortcut;
class KShortcutWidget;

class FontSizeCombo;
class KColorCombo2;

class StateCopy
{
public:
    typedef QList<StateCopy*> List;
    StateCopy(State *old = 0);
    ~StateCopy();
    State *oldState;
    State *newState;
    void copyBack();
};

class TagCopy
{
public:
    typedef QList<TagCopy*> List;
    TagCopy(Tag *old = 0);
    ~TagCopy();
    Tag *oldTag;
    Tag *newTag;
    StateCopy::List stateCopies;
    void copyBack();
    bool isMultiState();
};

class TagListViewItem : public QTreeWidgetItem
{
    friend class TagListDelegate;
public:
    TagListViewItem(QTreeWidget *parent, TagCopy *tagCopy);
    TagListViewItem(QTreeWidgetItem *parent, TagCopy *tagCopy);
    TagListViewItem(QTreeWidget *parent, QTreeWidgetItem *after, TagCopy *tagCopy);
    TagListViewItem(QTreeWidgetItem *parent, QTreeWidgetItem *after, TagCopy *tagCopy);
    TagListViewItem(QTreeWidget *parent, StateCopy *stateCopy);
    TagListViewItem(QTreeWidgetItem *parent, StateCopy *stateCopy);
    TagListViewItem(QTreeWidget *parent, QTreeWidgetItem *after, StateCopy *stateCopy);
    TagListViewItem(QTreeWidgetItem *parent, QTreeWidgetItem *after, StateCopy *stateCopy);
    ~TagListViewItem();
    TagCopy*   tagCopy()   {
        return m_tagCopy;
    }
    StateCopy* stateCopy() {
        return m_stateCopy;
    }
    bool isEmblemObligatory();
    TagListViewItem* lastChild();
    TagListViewItem* prevSibling();
    TagListViewItem* nextSibling();
    TagListViewItem* parent() const; // Reimplemented to cast the return value
    int width(const QFontMetrics &fontMetrics, const QTreeWidget *listView, int column) const;
    void setup();

private:
    TagCopy   *m_tagCopy;
    StateCopy *m_stateCopy;
};

Q_DECLARE_METATYPE(TagListViewItem*);

class TagListView : public QTreeWidget
{
    Q_OBJECT
public:
    TagListView(QWidget *parent = 0);
    ~TagListView();
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    TagListViewItem* currentItem() const; // Reimplemented to cast the return value
    TagListViewItem* firstChild() const; // Reimplemented to cast the return value
    TagListViewItem* lastItem() const; // Reimplemented to cast the return value
signals:
    void deletePressed();
    void doubleClickedItem();
};

/**
  * @author Sébastien Laoût
  */
class TagsEditDialog : public KDialog
{
    Q_OBJECT
public:
    explicit TagsEditDialog(QWidget *parent = 0, State *stateToEdit = 0, bool addNewTag = false);
    ~TagsEditDialog();
    State::List deletedStates() {
        return m_deletedStates;
    }
    State::List addedStates()   {
        return m_addedStates;
    }
    TagListViewItem* itemForState(State *state);

private slots:
    void newTag();
    void newState();
    void moveUp();
    void moveDown();
    void deleteTag();
    void renameIt();
    void capturedShortcut(const KShortcut &shortcut);
    void removeShortcut();
    void removeEmblem();
    void modified();
    void currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem* next = 0);
    void slotCancel();
    void slotOk();
    void selectUp();
    void selectDown();
    void selectLeft();
    void selectRight();
    void resetTreeSizeHint();
private:
    void loadBlankState();
    void loadStateFrom(State *state);
    void loadTagFrom(Tag *tag);
    void saveStateTo(State *state);
    void saveTagTo(Tag *tag);
    void ensureCurrentItemVisible();
    TagListView   *m_tags;
    KPushButton   *m_moveUp;
    KPushButton   *m_moveDown;
    KPushButton   *m_deleteTag;
    KLineEdit     *m_tagName;
    KShortcutWidget *m_shortcut;
    QPushButton   *m_removeShortcut;
    QCheckBox     *m_inherit;
    QGroupBox     *m_tagBox;
    QHBoxLayout   *m_tagBoxLayout;
    QGroupBox     *m_stateBox;
    QHBoxLayout   *m_stateBoxLayout;
    QLabel        *m_stateNameLabel;
    KLineEdit     *m_stateName;
    KIconButton   *m_emblem;
    QPushButton   *m_removeEmblem;
    QPushButton   *m_bold;
    QPushButton   *m_underline;
    QPushButton   *m_italic;
    QPushButton   *m_strike;
    KColorCombo2  *m_textColor;
    QFontComboBox *m_font;
    FontSizeCombo *m_fontSize;
    KColorCombo2  *m_backgroundColor;
    KLineEdit     *m_textEquivalent;
    QCheckBox     *m_onEveryLines;
    QCheckBox     *m_allowCrossRefernce;

    TagCopy::List m_tagCopies;
    State::List   m_deletedStates;
    State::List   m_addedStates;

    bool m_loading;
};


class TagListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    TagListDelegate(QWidget *parent = 0) : QItemDelegate(parent) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

};

#endif // TAGEDIT_H
