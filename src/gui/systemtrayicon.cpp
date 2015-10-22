// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "systemtrayicon.h"
#include "project.h"
#include "tomato.h"
#include "theme.h"
#include "utils.h"


SystemTrayIcon::SystemTrayIcon(Project *project, QObject *parent)
    : QSystemTrayIcon(parent)
    , m_project(project)
{
    setIcon(theme::icon(theme::IconApp));
    setVisible(true);

    m_statusAction = new QAction(this);
    m_statusAction->setEnabled(false);

    update();
}

void SystemTrayIcon::update()
{
    updateTrayIcon();
    updateTrayText();
}

void SystemTrayIcon::showWorkingTimeoutMessage()
{
    showMessage(tr("Tomato task tracker"),
                tr("The work time is over"),
                QSystemTrayIcon::Information);
}

void SystemTrayIcon::showRestingTimeoutMessage()
{
    showMessage(tr("Tomato task tracker"),
                tr("The rest time is over"),
                QSystemTrayIcon::Information);
}

void SystemTrayIcon::updateTrayText()
{
    QString text;
    if (!m_project->isOpen()) {
        text = tr("IDLE");
    }
    else {
        switch (m_project->tomato()->state()) {
        case Tomato::Idle:
            text = tr("IDLE: %1")
                   .arg(secsToTimeStr(m_project->tomato()->workingTime()));
            break;
        case Tomato::Working:
            text = tr("WORKING: %1")
                   .arg(secsToTimeStr(m_project->tomato()->workingTime()
                                      - m_project->tomato()->calcTomatoTime()));
            break;
        case Tomato::OverWorking:
            text = tr("OVERWORKING: %1")
                   .arg(secsToTimeStr(m_project->tomato()->calcTomatoTime()
                                      - m_project->tomato()->workingTime()));
            break;
        case Tomato::Resting:
            text = tr("RESTING: %1")
                   .arg(secsToTimeStr(m_project->tomato()->restingTime()
                                      - m_project->tomato()->calcTomatoTime()));
            break;
        case Tomato::OverResting:
            text = tr("OVERRESTING: %1")
                   .arg(secsToTimeStr(m_project->tomato()->calcTomatoTime()
                                      - m_project->tomato()->restingTime()));
            break;
        }
    }

    m_statusAction->setText(text);
    setToolTip(text);
}

void SystemTrayIcon::updateTrayIcon()
{
    if (!m_project->isOpen()) {
        setIcon(theme::icon(theme::IconStatusIdle));
        return;
    }

    switch (m_project->tomato()->state()) {
    case Tomato::Idle:
        setIcon(theme::icon(theme::IconStatusIdle));
        break;
    case Tomato::Working:
        setIcon(theme::icon(theme::IconStatusWorking));
        break;
    case Tomato::OverWorking:
        setIcon(theme::icon(theme::IconStatusTimeout));
        break;
    case Tomato::Resting:
        setIcon(theme::icon(theme::IconStatusResting));
        break;
    case Tomato::OverResting:
        setIcon(theme::icon(theme::IconStatusTimeout));
        break;
    }
}
