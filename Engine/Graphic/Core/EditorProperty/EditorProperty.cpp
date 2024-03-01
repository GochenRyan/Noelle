// Copyright 2024 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "EditorProperty.h"
#include "EditorPropertyCreator.h"
#include "PointerCastingUtils.h"

using namespace NoelleGraphic;

void ELabel::CallBackValue(std::string& value)
{
    if (m_pOwner)
    {
        m_pOwner->CallBackValue(this, &value);
    }
}

void ECheckBox::CallBackValue(bool value)
{
    if (m_pOwner)
    {
        m_pOwner->CallBackValue(this, &value);
    }
}

void ESlider::CallBackValue(uint32_t value)
{
    if (m_pOwner)
    {
        m_pOwner->CallBackValue(this, &value);
    }
}

void ECombo::CallBackValue(std::string& value)
{
    if (m_pOwner)
    {
        m_pOwner->CallBackValue(this, &value);
    }
}

void EColorTable::CallBackValue(NoelleMath::NVector<float, 4>& value)
{
    if (m_pOwner)
    {
        m_pOwner->CallBackValue(this, &value);
    }
}

void ETextBox::CallBackValue(std::string& value)
{
    if (m_pOwner)
    {
        m_pOwner->CallBackValue(this, &value);
    }
}

EditorProperty::EditorProperty(const std::string& name, Object* pOwner)
{
    m_pLabel = NoelleUtils::StaticUniquePointerCast<ELabel, EditorControl>(std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_LABEL, name)));
    m_pLabel->SetOwner(this);
    m_pLabel->SetValue((void*)(&name));

    m_pOwner = pOwner;
}

void EditorProperty::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

EBoolProperty::EBoolProperty(bool* b, std::string& name, Object* pOwner) : EditorProperty(name, pOwner)
{
    m_pCheckBox = NoelleUtils::StaticUniquePointerCast<ECheckBox, EditorControl>(std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_CHECK, name)));
    m_pCheckBox->SetOwner(this);
    m_pCheckBox->SetValue((void*)b);

    m_pOwner = pOwner;
}

template<typename T>
void CreateEditorElement(T value, Object* pOwner, ECollection* pParent, std::string& name, bool bRange, T max, T min, T step)
{
    
}