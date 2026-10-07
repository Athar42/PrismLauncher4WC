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
#include "ui/themes/SnowPainter.h"

#include <QEvent>
#include <QIcon>
#include <QPainter>
#include <QRandomGenerator>
#include <QRegion>
#include <QWidget>
#include <QtMath>

#include "Application.h"

namespace {
constexpr int kFrameIntervalMs = 33;    // ~30 fps
constexpr int kPixelsPerFlake = 20000;  // density: one flake per this many square pixels
constexpr int kMinFlakes = 10;
constexpr int kMaxFlakes = 40;

double randomBetween(double min, double max)
{
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}
}  // namespace

SnowPainter::SnowPainter(QWidget* target, QObject* parent) : QObject(parent), m_target(target), m_window(target->window())
{
    const qreal dpr = target->devicePixelRatioF();
    const QIcon logo = APPLICATION->logo();
    for (int size : { 10, 14, 19, 25 }) {
        m_spriteSizes.append(size);
        m_sprites.append(logo.pixmap(QSize(size, size), dpr));
    }

    m_timer.setInterval(kFrameIntervalMs);
    connect(&m_timer, &QTimer::timeout, this, &SnowPainter::tick);

    m_target->installEventFilter(this);
    if (m_window && m_window != m_target) {
        m_window->installEventFilter(this);
    }

    updateRunning();
}

bool SnowPainter::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type()) {
        case QEvent::Show:
        case QEvent::Hide:
        case QEvent::WindowStateChange:
            updateRunning();
            break;
        case QEvent::Resize:
            if (watched == m_target) {
                syncFlakeCount();
            }
            break;
        default:
            break;
    }
    return QObject::eventFilter(watched, event);
}

void SnowPainter::updateRunning()
{
    const bool shouldRun = m_target && m_target->isVisible() && !(m_window && m_window->isMinimized());
    if (shouldRun && !m_timer.isActive()) {
        syncFlakeCount();
        m_clock.restart();
        m_timer.start();
    } else if (!shouldRun && m_timer.isActive()) {
        m_timer.stop();
    }
}

void SnowPainter::syncFlakeCount()
{
    if (!m_target) {
        return;
    }
    const int area = m_target->width() * m_target->height();
    const int wanted = qBound(kMinFlakes, area / kPixelsPerFlake, kMaxFlakes);
    while (m_flakes.size() < wanted) {
        m_flakes.append(makeFlake(true));
    }
    if (m_flakes.size() > wanted) {
        m_flakes.resize(wanted);
    }
}

SnowPainter::Flake SnowPainter::makeFlake(bool anywhere) const
{
    Flake flake;
    flake.sprite = QRandomGenerator::global()->bounded(static_cast<int>(m_sprites.size()));
    const int size = m_spriteSizes[flake.sprite];
    flake.x = randomBetween(0.0, 1.0);
    flake.y = anywhere && m_target ? randomBetween(-size, m_target->height()) : -size;
    // bigger flakes are "closer": they fall faster and are more visible
    flake.speed = 12.0 + size * 1.3 + randomBetween(-4.0, 4.0);
    flake.swayAmp = randomBetween(4.0, 14.0);
    flake.swayFreq = randomBetween(0.6, 1.4);
    flake.swayPhase = randomBetween(0.0, 2 * M_PI);
    flake.angle = randomBetween(0.0, 360.0);
    flake.spin = randomBetween(-25.0, 25.0);
    flake.opacity = 0.15 + size / 100.0 + randomBetween(0.0, 0.1);
    return flake;
}

QRect SnowPainter::flakeRect(const Flake& flake) const
{
    // half of the diagonal, so the rect also covers the rotated sprite
    const int half = qCeil(m_spriteSizes[flake.sprite] * 0.71) + 1;
    const double cx = flake.x * m_target->width() + flake.swayAmp * qSin(flake.swayPhase);
    return QRect(qFloor(cx) - half, qFloor(flake.y) - half, 2 * half + 1, 2 * half + 1);
}

void SnowPainter::tick()
{
    if (!m_target) {
        m_timer.stop();
        return;
    }
    // clamp the step so a stalled event loop doesn't make flakes jump
    const double dt = qMin(m_clock.restart() / 1000.0, 0.1);
    const int height = m_target->height();

    // only repaint the areas the flakes leave and enter, not the whole view
    QRegion dirty;
    for (auto& flake : m_flakes) {
        dirty += flakeRect(flake);
        flake.y += flake.speed * dt;
        flake.swayPhase += flake.swayFreq * dt;
        flake.angle += flake.spin * dt;
        if (flake.y - m_spriteSizes[flake.sprite] > height) {
            flake = makeFlake(false);
        }
        dirty += flakeRect(flake);
    }
    m_target->update(dirty);
}

void SnowPainter::paint(QPainter* painter)
{
    if (!m_target || m_flakes.isEmpty()) {
        return;
    }
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    const QTransform base = painter->transform();
    const int width = m_target->width();
    for (const auto& flake : m_flakes) {
        const int size = m_spriteSizes[flake.sprite];
        const QPixmap& sprite = m_sprites[flake.sprite];
        painter->setOpacity(flake.opacity);
        painter->setTransform(base);
        painter->translate(flake.x * width + flake.swayAmp * qSin(flake.swayPhase), flake.y);
        painter->rotate(flake.angle);
        painter->drawPixmap(QRectF(-size / 2.0, -size / 2.0, size, size), sprite, QRectF(sprite.rect()));
    }
    painter->restore();
}
