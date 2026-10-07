// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Winter Launcher - WinterCup fork of Prism Launcher
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "WinterTheme.h"

#include <QLocale>

namespace {
// These names are not part of the upstream translations, so pick French or English here
bool isFrench()
{
    return QLocale().language() == QLocale::French;
}
}  // namespace

// ---------------------------------------------------------------------------
// Winter Dark
// ---------------------------------------------------------------------------

QString WinterDarkTheme::id()
{
    return "winter_dark";
}

QString WinterDarkTheme::name()
{
    return isFrench() ? QStringLiteral("Hiver (sombre)") : QStringLiteral("Winter (dark)");
}

QPalette WinterDarkTheme::colorScheme()
{
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(22, 34, 47));
    palette.setColor(QPalette::WindowText, QColor(230, 241, 250));
    palette.setColor(QPalette::Base, QColor(15, 25, 35));
    palette.setColor(QPalette::AlternateBase, QColor(21, 33, 45));
    palette.setColor(QPalette::ToolTipBase, QColor(31, 95, 139));
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, QColor(230, 241, 250));
    palette.setColor(QPalette::Button, QColor(28, 45, 61));
    palette.setColor(QPalette::ButtonText, QColor(230, 241, 250));
    palette.setColor(QPalette::BrightText, QColor(255, 107, 107));
    palette.setColor(QPalette::Link, QColor(92, 196, 238));
    palette.setColor(QPalette::Highlight, QColor(42, 143, 208));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    palette.setColor(QPalette::PlaceholderText, QColor(110, 130, 149));
    return fadeInactive(palette, fadeAmount(), fadeColor());
}

double WinterDarkTheme::fadeAmount()
{
    return 0.5;
}

QColor WinterDarkTheme::fadeColor()
{
    return QColor(22, 34, 47);
}

bool WinterDarkTheme::hasStyleSheet()
{
    return true;
}

QString WinterDarkTheme::appStyleSheet()
{
    return "QToolTip { color: #ffffff; background-color: #1f5f8b; border: 1px solid #5cc4ee; }";
}

QString WinterDarkTheme::tooltip()
{
    return "";
}

// ---------------------------------------------------------------------------
// Winter Light
// ---------------------------------------------------------------------------

QString WinterLightTheme::id()
{
    return "winter_light";
}

QString WinterLightTheme::name()
{
    return isFrench() ? QStringLiteral("Hiver (clair)") : QStringLiteral("Winter (light)");
}

QPalette WinterLightTheme::colorScheme()
{
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(242, 248, 252));
    palette.setColor(QPalette::WindowText, QColor(16, 42, 67));
    palette.setColor(QPalette::Base, QColor(255, 255, 255));
    palette.setColor(QPalette::AlternateBase, QColor(232, 242, 250));
    palette.setColor(QPalette::ToolTipBase, QColor(15, 76, 117));
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, QColor(16, 42, 67));
    palette.setColor(QPalette::Button, QColor(227, 239, 248));
    palette.setColor(QPalette::ButtonText, QColor(16, 42, 67));
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(0, 118, 192));
    palette.setColor(QPalette::Highlight, QColor(15, 124, 192));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    palette.setColor(QPalette::PlaceholderText, QColor(122, 143, 163));
    return fadeInactive(palette, fadeAmount(), fadeColor());
}

double WinterLightTheme::fadeAmount()
{
    return 0.5;
}

QColor WinterLightTheme::fadeColor()
{
    return QColor(242, 248, 252);
}

bool WinterLightTheme::hasStyleSheet()
{
    return true;
}

QString WinterLightTheme::appStyleSheet()
{
    return "QToolTip { color: #ffffff; background-color: #0f4c75; border: 1px solid #36acd1; }";
}

QString WinterLightTheme::tooltip()
{
    return "";
}
