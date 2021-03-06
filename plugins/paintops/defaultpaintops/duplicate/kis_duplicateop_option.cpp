/*
 *  Copyright (c) 2008 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "kis_duplicateop_option.h"
#include <klocalizedstring.h>

#include <QWidget>
#include <QRadioButton>

#include "ui_wdgduplicateop.h"
#include <kis_image.h>

class KisDuplicateOpOptionsWidget: public QWidget, public Ui::DuplicateOpOptionsWidget
{
public:
    KisDuplicateOpOptionsWidget(QWidget *parent = 0)
        : QWidget(parent) {
        setupUi(this);
    }
    KisImageWSP m_image;
protected:
    void showEvent(QShowEvent* event) {
        QWidget::showEvent(event);
        //cbPerspective->setEnabled(m_image && m_image->perspectiveGrid() && m_image->perspectiveGrid()->countSubGrids() == 1);
        cbPerspective->setVisible(false); // XXX: Until perspective cloning works again!
    }
};


KisDuplicateOpOption::KisDuplicateOpOption()
    : KisPaintOpOption(KisPaintOpOption::COLOR, false)
{
    setObjectName("KisDuplicateOpOption");

    m_checkable = false;
    m_optionWidget = new KisDuplicateOpOptionsWidget();

    connect(m_optionWidget->cbHealing, SIGNAL(toggled(bool)), SLOT(emitSettingChanged()));
    connect(m_optionWidget->cbPerspective, SIGNAL(toggled(bool)), SLOT(emitSettingChanged()));
    connect(m_optionWidget->cbSourcePoint, SIGNAL(toggled(bool)), SLOT(emitSettingChanged()));
    connect(m_optionWidget->chkCloneProjection, SIGNAL(toggled(bool)), SLOT(emitSettingChanged()));

    setConfigurationPage(m_optionWidget);
}


KisDuplicateOpOption::~KisDuplicateOpOption()
{
}

bool KisDuplicateOpOption::healing() const
{
    return m_optionWidget->cbHealing->isChecked();
}

void KisDuplicateOpOption::setHealing(bool healing)
{
    m_optionWidget->cbHealing->setChecked(healing);
}

bool KisDuplicateOpOption::correctPerspective() const
{
    return m_optionWidget->cbPerspective->isChecked();
}

void KisDuplicateOpOption::setPerspective(bool perspective)
{
    m_optionWidget->cbPerspective->setChecked(perspective);
}

bool KisDuplicateOpOption::moveSourcePoint() const
{
    return m_optionWidget->cbSourcePoint->isChecked();
}

void KisDuplicateOpOption::setMoveSourcePoint(bool move)
{
    m_optionWidget->cbSourcePoint->setChecked(move);
}

bool KisDuplicateOpOption::cloneFromProjection() const
{
    return m_optionWidget->chkCloneProjection->isChecked();
}

void KisDuplicateOpOption::setCloneFromProjection(bool cloneFromProjection)
{
    m_optionWidget->chkCloneProjection->setChecked(cloneFromProjection);
}

void KisDuplicateOpOption::writeOptionSetting(KisPropertiesConfiguration* setting) const
{
    setting->setProperty(DUPLICATE_HEALING, healing());
    setting->setProperty(DUPLICATE_CORRECT_PERSPECTIVE, correctPerspective());
    setting->setProperty(DUPLICATE_MOVE_SOURCE_POINT, moveSourcePoint());
    setting->setProperty(DUPLICATE_CLONE_FROM_PROJECTION, cloneFromProjection());
}

void KisDuplicateOpOption::readOptionSetting(const KisPropertiesConfiguration* setting)
{
    m_optionWidget->cbHealing->setChecked(setting->getBool(DUPLICATE_HEALING, false));
    m_optionWidget->cbPerspective->setChecked(setting->getBool(DUPLICATE_CORRECT_PERSPECTIVE, false));
    m_optionWidget->cbSourcePoint->setChecked(setting->getBool(DUPLICATE_MOVE_SOURCE_POINT, true));
    m_optionWidget->chkCloneProjection->setChecked(setting->getBool(DUPLICATE_CLONE_FROM_PROJECTION, false));
}

void KisDuplicateOpOption::setImage(KisImageWSP image)
{
    m_optionWidget->m_image = image;
}
