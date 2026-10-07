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
#pragma once

#include <QElapsedTimer>
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QPointer>
#include <QTimer>

class QPainter;
class QWidget;

// Paints snowflakes (the launcher logo) falling in the background of a widget.
// Built to stay light: pre-rendered flakes, ~30 fps, only the flake areas are
// repainted, and the timer is fully stopped while the window is hidden or minimized.
class SnowPainter : public QObject {
    Q_OBJECT
   public:
    explicit SnowPainter(QWidget* target, QObject* parent = nullptr);
    ~SnowPainter() override = default;

    void paint(QPainter* painter);

   protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

   private:
    struct Flake {
        double x;          // relative horizontal position, 0..1
        double y;          // vertical position in pixels
        double speed;      // pixels per second
        double swayAmp;    // horizontal sway amplitude in pixels
        double swayFreq;   // sway frequency in radians per second
        double swayPhase;  // current sway phase
        double angle;      // rotation in degrees
        double spin;       // rotation speed in degrees per second
        double opacity;
        int sprite;        // index in m_sprites
    };

    void tick();
    void updateRunning();
    void syncFlakeCount();
    Flake makeFlake(bool anywhere) const;
    QRect flakeRect(const Flake& flake) const;

    QPointer<QWidget> m_target;
    QPointer<QWidget> m_window;
    QTimer m_timer;
    QElapsedTimer m_clock;
    QList<QPixmap> m_sprites;
    QList<int> m_spriteSizes;
    QList<Flake> m_flakes;
};
