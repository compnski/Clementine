/* This file is part of Clementine.

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "songinfotextview.h"

#include <QApplication>
#include <QMenu>
#include <QSettings>
#include <QWheelEvent>
#include <QtDebug>

const qreal SongInfoTextView::kDefaultFontSize = 8.5;
const char* SongInfoTextView::kSettingsGroup = "SongInfo";

SongInfoTextView::SongInfoTextView(QWidget* parent)
  : QTextBrowser(parent),
    last_width_(-1)
{
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  setOpenExternalLinks(true);
  ReloadSettings();
}

void SongInfoTextView::ReloadSettings() {
  QSettings s;
  s.beginGroup(kSettingsGroup);

  qreal size = s.value("font_size", kDefaultFontSize).toReal();
  QFont font;
  font.setPointSizeF(size);
  document()->setDefaultFont(font);
}

void SongInfoTextView::resizeEvent(QResizeEvent* e) {
  const int w = qMax(100, width());
  if (w == last_width_)
    return;
  last_width_ = w;

  document()->setTextWidth(w);
  setMinimumHeight(document()->size().height());

  QTextEdit::resizeEvent(e);
}

QSize SongInfoTextView::sizeHint() const {
  return minimumSize();
}

void SongInfoTextView::wheelEvent(QWheelEvent* e) {
  e->ignore();
}

void SongInfoTextView::contextMenuEvent(QContextMenuEvent* e) {
  QMenu* menu = createStandardContextMenu(e->pos());
  menu->setAttribute(Qt::WA_DeleteOnClose);

  menu->addAction(tr("Change font size..."), this, SIGNAL(ShowSettingsDialog()));

  menu->popup(e->globalPos());
}

void SongInfoTextView::SetHtml(const QString& html) {
  QString copy(html.trimmed());

  // Simplify newlines, and convert them to <p>
  copy.replace(QRegExp("[\\r\\n]+"), "\n");
  copy.replace(QRegExp("([^>])[\\t ]*\\n"), "\\1<p>");

  // Strip any newlines from the end
  copy.replace(QRegExp("((<\\s*br\\s*/?\\s*>)|(<\\s*/?\\s*p\\s*/?\\s*>))+$"), "");

  setHtml(copy);
}
