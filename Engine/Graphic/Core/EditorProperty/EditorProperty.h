// Copyright 2023 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "Graphic.h"
#include "Core/Object.h"
#include "NVector.h"

#include <string>
#include <vector>
#include <memory>

namespace NoelleGraphic
{
    class EditorProperty;
    class EditorPropertyCreator;

    class GRAPHIC_API IEditorElement
    {
    public:
        IEditorElement() = default;

        virtual bool IsCollection()
        {
            return false;
        }

        virtual ~IEditorElement() = default;
    };

    // #region Logic Control

    class GRAPHIC_API EditorControl: public IEditorElement
    {
    public:
        enum CONTROL_TYPE
        {
            CT_CHECK,
            CT_LABEL,
            CT_TEXT,
            CT_COMBO,
            CT_SLIDER,
            CT_COLOR,
            CT_VIEW,
            CT_COLLECTION
        };

        EditorControl(const std::string& name)
        {
            m_sName = name; 
        }

        virtual ~EditorControl() = default;

        void SetOwner(EditorProperty* editorProperty)
        {
            m_pOwner = editorProperty;
        }

        virtual void SetValue(void* value)
        {
        }

        std::string& GetName()
        {
            return m_sName;
        }

    protected:
        EditorProperty* m_pOwner;
        std::string m_sName;
    };

    class GRAPHIC_API ELabel: public EditorControl
    {
    public:
        ELabel(const std::string& name): EditorControl(name)
        {
        }
        
        virtual ~ELabel() = default;

        virtual void CallBackValue(std::string& value);

        virtual void SetValue(void* value)
        {
            m_Content = *static_cast<std::string*>(value);
        }
    protected:
        std::string m_Content;
    };

    class GRAPHIC_API ECheckBox: public EditorControl
    {
    public:
        ECheckBox(const std::string& name): EditorControl(name)
        {
        }

        virtual ~ECheckBox() = default;

        virtual void CallBackValue(bool value);

        virtual void SetValue(void* value)
        {
            m_Content = *static_cast<bool*>(value);
        }
    protected:
        bool m_Content;
    };

    class GRAPHIC_API ESlider: public EditorControl
    {
    public:
        ESlider(std::string& name, uint32_t uiMin, uint32_t uiMax, uint32_t uiStep): EditorControl(name)
        {
            m_uiMin = uiMin;
            m_uiMax = uiMax;
            m_uiStep = uiStep;
        }

        virtual ~ESlider() = default;

        virtual void CallBackValue(uint32_t value);
    protected:
        uint32_t m_uiMin;
        uint32_t m_uiMax;
        uint32_t m_uiStep;
        uint32_t m_uiContent;
    };

    class GRAPHIC_API EViewWindow: public EditorControl
    {
        
    };

    class GRAPHIC_API ECombo: public EditorControl
    {
    public:
        ECombo(std::string& name): EditorControl(name)
        {

        }

        virtual ~ECombo() = default;

        virtual void AddOption(std::string option)
        {
        }

        virtual void AddOptions(std::vector<std::string> options)
        {
        }

        virtual void CallBackValue(std::string& value);
    protected:
        std::vector<const char*> m_Options;
    };

    class GRAPHIC_API EColorTable: public EditorControl
    {
    public:
        EColorTable(std::string& name): EditorControl(name)
        {

        }

        virtual ~EColorTable() = default;

        virtual void CallBackValue(NoelleMath::NVector<float, 4>& value);
    };

    class GRAPHIC_API ETextBox: public EditorControl
    {
    public:
        ETextBox(std::string& name): EditorControl(name)
        {

        }

        virtual ~ETextBox() = default;

        virtual void CallBackValue(std::string& value);
    protected:
        std::string m_Content;
    };

    class GRAPHIC_API ECollection: public EditorControl
    {
    public:
        ECollection(std::string& name): EditorControl(name)
        {

        }

        virtual ~ECollection() = default;

        virtual bool IsCollection()
        {
            return true;
        }

        void AddElement(EditorControl& element)
        {
            m_Collection.push_back(element);
        }

    protected:
        std::vector<EditorControl> m_Collection;
    };

    // #endregion

    // #region Editor Property

    class GRAPHIC_API EditorProperty: public IEditorElement
    {
    public:
        EditorProperty(const std::string& name, Object* pOwner);

        virtual ~EditorProperty()
        {
            m_pOwner = nullptr;
        }

        virtual void CallBackValue(EditorControl* pControl, void* pData);
    protected:
        std::unique_ptr<ELabel> m_pLabel;
        Object* m_pOwner;
    };

    class GRAPHIC_API EBoolProperty: public EditorProperty
    {
    public:
        EBoolProperty(bool* b, std::string& name, Object* pOwner);

        void NewFunction(std::string & name);

        virtual ~EBoolProperty()
        {
            m_pOwner = nullptr;
        }

    protected:
        std::unique_ptr<ECheckBox> m_pCheckBox;
    };

    // class GRAPHIC_API EResourceProperty: public EditorProperty
    // {
    // public:
    //     EResourceProperty(std::string& name, Object* pOwner)
    // };

    class GRAPHIC_API EColorProperty: public EditorProperty
    {
    public:
        EColorProperty(NoelleMath::NVector<float, 4>* color, std::string& name, Object* pOwner);

        ~EColorProperty() = default;
    protected:
        std::unique_ptr<EColorTable> m_pColorTable;
    };

    class GRAPHIC_API EEnumProperty: public EditorProperty
    {
    public:
        EEnumProperty(unsigned int* pSelection, std::string& name, Object* pOwner);

        ~EEnumProperty()
        {
            m_pSelection = nullptr;
        }

        void AddEnumString(std::vector<std::string>& enumStr);
    protected:
        std::unique_ptr<ECombo> m_pCombo;
        unsigned int* m_pSelection;
    };

    template <typename T>
    class GRAPHIC_API EValueProperty: public EditorProperty
    {
    public:
        EValueProperty(T* pData, std::string& name, Object* pOwner);

        ~EValueProperty() = default;
    protected:
        std::unique_ptr<ESlider> m_pSlider;
    };

    class GRAPHIC_API EIntProperty: public EValueProperty<int>
    {
    public:
        EIntProperty(std::string& name, Object* pOwner);

        ~EIntProperty() = default;
    };

    class GRAPHIC_API EUIntProperty: public EValueProperty<uint32_t>
    {
    public:
        EUIntProperty(std::string& name, Object* pOwner);

        ~EUIntProperty() = default;
    };

    class GRAPHIC_API EStringProperty: public EditorProperty
    {
    public:
        EStringProperty(std::string* pStr, std::string& name, Object* pOwner);

        ~EStringProperty() = default;
    protected:
        std::unique_ptr<ETextBox> m_pTextBox;
    };

    class GRAPHIC_API EFloatProperty: public EValueProperty<float>
    {
    public:
        EFloatProperty(float* pData, std::string& name, Object* pOwner);

        ~EFloatProperty() = default;
    };

    class GRAPHIC_API EVector3Property: public EditorProperty
    {
    public:
        EVector3Property(NoelleMath::NVector<float, 3> pVector, std::string& name, Object* pOwner);

        ~EVector3Property() = default;
    protected:
        std::unique_ptr<ELabel> m_pLabelX;
        std::unique_ptr<ESlider> m_pSliderX;
        std::unique_ptr<ETextBox> m_pTextBoxX;

        std::unique_ptr<ELabel> m_pLabelY;
        std::unique_ptr<ESlider> m_pSliderY;
        std::unique_ptr<ETextBox> m_pTextBoxY;

        std::unique_ptr<ELabel> m_pLabelZ;
        std::unique_ptr<ESlider> m_pSliderZ;
        std::unique_ptr<ETextBox> m_pTextBoxZ;
    };

    class GRAPHIC_API ETransformProperty: public EditorProperty
    {
    public:
        ETransformProperty(NoelleMath::NTransform<float> pTransform, std::string& name, Object* pOwner);

        ~ETransformProperty() = default;
    protected:
        std::unique_ptr<ELabel> m_pLabelPosition;
        std::unique_ptr<ETextBox> m_pTextBoxPositionX;
        std::unique_ptr<ELabel> m_pLabelPositionX;
        std::unique_ptr<ETextBox> m_pTextBoxPositionY;
        std::unique_ptr<ELabel> m_pLabelPositionY;
        std::unique_ptr<ETextBox> m_pTextBoxPositionZ;
        std::unique_ptr<ELabel> m_pLabelPositionZ;

        std::unique_ptr<ELabel> m_pLabelRotation;
        std::unique_ptr<ETextBox> m_pTextBoxRotationX;
        std::unique_ptr<ELabel> m_pLabelRotationX;
        std::unique_ptr<ETextBox> m_pTextBoxRotationY;
        std::unique_ptr<ELabel> m_pLabelRotationY;
        std::unique_ptr<ETextBox> m_pTextBoxRotationZ;
        std::unique_ptr<ELabel> m_pLabelRotationZ;

        std::unique_ptr<ELabel> m_pLabelScale;
        std::unique_ptr<ETextBox> m_pTextBoxScaleX;
        std::unique_ptr<ELabel> m_pLabelScaleX;
        std::unique_ptr<ETextBox> m_pTextBoxScaleY;
        std::unique_ptr<ELabel> m_pLabelScaleY;
        std::unique_ptr<ETextBox> m_pTextBoxScaleZ;
        std::unique_ptr<ELabel> m_pLabelScaleZ;
    };

    class GRAPHIC_API ENoDefineProperty: public EditorProperty
    {
    public:
        ENoDefineProperty() : EditorProperty("", nullptr)
        {

        }

        ~ENoDefineProperty()
        {

        }
    };

    // #endregion

    template<typename T>
    void CreateEditorElement(T value, Object* pOwner, ECollection* pParent, std::string& name, bool bRange, T max, T min, T step);
} // namespace name
