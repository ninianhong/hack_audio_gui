#include "hack_audio_FlexBox.h"

HackAudio::FlexBox::FlexBox()
{

    flexDirection  = juce::FlexBox::Direction::row;
    alignItems     = juce::FlexBox::AlignItems::center;
    alignContent   = juce::FlexBox::AlignContent::center;
    justifyContent = juce::FlexBox::JustifyContent::spaceAround;

    flexBoxBounds.setBounds(0, 0, 0, 0);

    defaultFlexSettings.alignSelf = juce::FlexItem::AlignSelf::center;

    resizeGuard = false;

    validBounds = false;

}

HackAudio::FlexBox::~FlexBox()
{

}

void HackAudio::FlexBox::setDefaultItem(juce::FlexItem item, bool updateExistingItems)
{
    defaultFlexSettings = item;

    if (updateExistingItems)
    {
        applyGlobalFlexItem(item);
    }

}

void HackAudio::FlexBox::addComponent(juce::Component* component, int customOrder)
{

    juce::FlexItem itemToAdd = defaultFlexSettings;
    itemToAdd.associatedComponent = component;
    itemToAdd.width  = component->getWidth();
    itemToAdd.height = component->getHeight();
    itemToAdd.order  = customOrder;
    items.add(itemToAdd);

    component->addComponentListener(this);

    applyLayout();

}

void HackAudio::FlexBox::removeComponent(juce::Component* component)
{
    for (int i = 0; i < items.size(); ++i)
    {
        juce::FlexItem& fi = items.getReference(i);

        if (fi.associatedComponent == component || !fi.associatedComponent)
        {
            items.remove(i);
        }
    }
}

void HackAudio::FlexBox::setDirection(juce::FlexBox::Direction newDirection)
{
    flexDirection = newDirection;
    applyLayout();
}

juce::FlexBox::Direction HackAudio::FlexBox::getDirection()
{
    return flexDirection;
}

void HackAudio::FlexBox::setWrap(juce::FlexBox::Wrap newWrap)
{
    flexWrap = newWrap;
    applyLayout();
}

juce::FlexBox::Wrap HackAudio::FlexBox::getWrap()
{
    return flexWrap;
}

void HackAudio::FlexBox::setAlignContent(juce::FlexBox::AlignContent newAlignment)
{
    alignContent = newAlignment;
    applyLayout();
}

juce::FlexBox::AlignContent HackAudio::FlexBox::getAlignContent()
{
    return alignContent;
}

void HackAudio::FlexBox::setAlignItems(juce::FlexBox::AlignItems newAlignment)
{
    alignItems = newAlignment;
    applyLayout();
}

juce::FlexBox::AlignItems HackAudio::FlexBox::getAlignItems()
{
    return alignItems;
}

void HackAudio::FlexBox::setJustifyContent(juce::FlexBox::JustifyContent newJustification)
{
    justifyContent = newJustification;
    applyLayout();
}

juce::FlexBox::JustifyContent HackAudio::FlexBox::getJustifyContent()
{
    return justifyContent;
}

juce::Array<juce::FlexItem> HackAudio::FlexBox::getItems()
{
    return items;
}

void HackAudio::FlexBox::applyItemFlex(float newFlexGrow)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).flexGrow = newFlexGrow;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemFlex(float newFlexGrow, float newFlexShrink)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).flexGrow   = newFlexGrow;
        items.getReference(i).flexShrink = newFlexShrink;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemFlex(float newFlexGrow, float newFlexShrink, float newFlexBasis)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).flexGrow   = newFlexGrow;
        items.getReference(i).flexShrink = newFlexShrink;
        items.getReference(i).flexBasis  = newFlexBasis;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemWidth(float newWidth)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).width = newWidth;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemHeight(float newHeight)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).width = newHeight;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyItemMargin(juce::FlexItem::Margin newMargin)
{
    for (int i = 0; i < items.size(); ++i)
    {
        items.getReference(i).margin = newMargin;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyGlobalFlexItem(juce::FlexItem newFlexProperties)
{
    for (int i = 0; i < items.size(); ++i)
    {
        juce::Component * c = items[i].associatedComponent;
        items[i] = newFlexProperties;
        items.getReference(i).associatedComponent = c;
    }
    applyLayout();
}

void HackAudio::FlexBox::applyBounds(juce::Rectangle<int> bounds)
{
    flexBoxBounds = bounds;
    validBounds = true;
    applyLayout();
}

void HackAudio::FlexBox::applyLayout()
{

    if (!validBounds) { return; }

    resizeGuard = true;
    performLayout(flexBoxBounds);
    resizeGuard = false;

}

void HackAudio::FlexBox::componentMovedOrResized(juce::Component &component, bool wasMoved, bool wasResized)
{

    if (resizeGuard) { return; }

    if (component.getWidth() == 0 || component.getHeight() == 0)
    {

        for (int i = 0; i < items.size(); ++i)
        {

            juce::FlexItem& fi = items.getReference(i);

            if (fi.associatedComponent == &component)
            {

                component.getProperties().set(juce::Identifier("flexIndexCache"), juce::var(i));

                items.remove(i);

            }
        }
    }
    else
    {
        if(component.getProperties().contains(juce::Identifier("flexIndexCache")))
        {

            juce::FlexItem fi = defaultFlexSettings;

            fi.associatedComponent = &component;

            fi.width  = component.getWidth();
            fi.height = component.getHeight();

            int index = component.getProperties().getWithDefault(juce::Identifier("flexIndexCache"), juce::var(0));
            items.insert(index, fi);

            component.getProperties().remove(juce::Identifier("flexIndexCache"));

        }
        else
        {
            for (int i = 0; i < items.size(); ++i)
            {
                juce::FlexItem& fi = items.getReference(i);

                if (fi.associatedComponent == &component)
                {
                    fi.width  = component.getWidth();
                    fi.height = component.getHeight();
                }
            }
        }
    }

    applyLayout();

}

void HackAudio::FlexBox::componentVisibilityChanged(juce::Component &component)
{

    if (component.isVisible())
    {
        juce::FlexItem fi = defaultFlexSettings;

        fi.associatedComponent = &component;
        fi.width = component.getProperties().getWithDefault(juce::Identifier("flexWidthCache"), juce::var(0));
        fi.height = component.getProperties().getWithDefault(juce::Identifier("flexHeightCache"), juce::var(0));

        component.getProperties().remove(juce::Identifier("flexWidthCache"));
        component.getProperties().remove(juce::Identifier("flexHeightCache"));

        int index = component.getProperties().getWithDefault(juce::Identifier("flexIndexCache"), juce::var(0));
        items.insert(index, fi);
    }
    else
    {
        for (int i = 0; i < items.size(); ++i)
        {
            juce::FlexItem& fi = items.getReference(i);

            if (fi.associatedComponent == &component)
            {
                component.getProperties().set(juce::Identifier("flexIndexCache"), juce::var(i));
                component.getProperties().set(juce::Identifier("flexWidthCache"), juce::var(component.getWidth()));
                component.getProperties().set(juce::Identifier("flexHeightCache"), juce::var(component.getHeight()));

                items.remove(i);
            }
        }
    }

    applyLayout();

}
