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

void EColorTable::CallBackValue(NoelleMath::NVec4f& value)
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
    m_pLabel = NoelleUtils::StaticUniquePointerCast<ELabel, EditorControl>(
        std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_LABEL, name)));
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
    m_pCheckBox = NoelleUtils::StaticUniquePointerCast<ECheckBox, EditorControl>(
        std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_CHECK, name)));
    m_pCheckBox->SetOwner(this);
    m_pCheckBox->SetValue((void*)b);
}

void EBoolProperty::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

EColorProperty::EColorProperty(NoelleMath::NVec4f* color, std::string& name, Object* pOwner) : EditorProperty(name, pOwner)
{
    m_pColorTable = NoelleUtils::StaticUniquePointerCast<EColorTable, EditorControl>(
        std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_COLOR, name)));
    m_pColorTable->SetOwner(this);
    m_pColorTable->SetValue((void*)color);
}

void EColorProperty::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

EEnumProperty::EEnumProperty(const std::vector<std::string>& data,unsigned int* pSelection, std::string& name, Object* pOwner) : EditorProperty(name, pOwner)
{
    m_pCombo = NoelleUtils::StaticUniquePointerCast<ECombo, EditorControl>(
        std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_COMBO, name)));
    m_pCombo->SetOwner(this);
    m_pCombo->AddOptions(data);
    m_pCombo->SetValue((void*)pSelection);
}

void NoelleGraphic::EEnumProperty::AddEnumString(std::vector<std::string>& enumStr)
{
    m_pCombo->AddOptions(enumStr);
}

void EEnumProperty::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

template <typename T>
EValueProperty<T>::EValueProperty<T>(T* pData, uint32_t uiMin, uint32_t uiMax, uint32_t uiStep, std::string& name, Object* pOwner) : EditorProperty(name, pOwner)
{
    m_pSlider = NoelleUtils::StaticUniquePointerCast<ESlider, EditorControl>(
        std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_SLIDER, name)));
    m_pSlider->SetRange(uiMin, uiMax, uiStep);
    m_pSlider->SetValue((void*)pData);

    m_pTextBox = NoelleUtils::StaticUniquePointerCast<ETextBox, EditorControl>(
        std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_TEXT, name)));
}

template <typename T>
void EValueProperty<T>::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

EStringProperty::EStringProperty(std::string* pStr, std::string& name, Object* pOwner) : EditorProperty(name, pOwner)
{
    m_pTextBox = NoelleUtils::StaticUniquePointerCast<ETextBox, EditorControl>(
        std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_TEXT, name)));

    m_pTextBox->SetValue((void*)pStr);
}

void EStringProperty::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

EVector3Property::EVector3Property(NoelleMath::NVec3f* pVector, NoelleMath::NVec3f min, NoelleMath::NVec3f max, NoelleMath::NVec3f step, std::string& name, Object* pOwner) : EditorProperty(name, pOwner)
{
    //m_pLabelX = NoelleUtils::StaticUniquePointerCast<ELabel, EditorControl>(
    //    std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_LABEL, "X")));
    //m_pSlider = NoelleUtils::StaticUniquePointerCast<ESlider, EditorControl>(
    //    std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_SLIDER, name)));
    //m_pTextBoxX = NoelleUtils::StaticUniquePointerCast<ETextBox, EditorControl>(
    //    std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_TEXT, name + "X")));

    //m_pLabelY = NoelleUtils::StaticUniquePointerCast<ELabel, EditorControl>(
    //    std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_LABEL, "Y")));

    //m_pTextBoxY = NoelleUtils::StaticUniquePointerCast<ETextBox, EditorControl>(
    //    std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_TEXT, name + "Y")));

    //m_pLabelZ = NoelleUtils::StaticUniquePointerCast<ELabel, EditorControl>(
    //    std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_LABEL, "Z")));

    //m_pTextBoxZ = NoelleUtils::StaticUniquePointerCast<ETextBox, EditorControl>(
    //    std::move(EditorPropertyCreator::GetInstance().CreateUIProperty(EditorControl::CONTROL_TYPE::CT_TEXT, name + "Z")));
}

void EVector3Property::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

ETransformProperty::ETransformProperty(NoelleMath::NTransform<float> pTransform, std::string& name, Object* pOwner) : EditorProperty(name, pOwner)
{

}

void ETransformProperty::CallBackValue(EditorControl* pControl, void* pData)
{
    m_pOwner->ValueChanged(pControl->GetName(), pData);
}

template<typename T>
void CreateEditorElement(T value, Object* pOwner, ECollection* pParent, std::string& name, bool bRange, T max, T min, T step)
{
    
}