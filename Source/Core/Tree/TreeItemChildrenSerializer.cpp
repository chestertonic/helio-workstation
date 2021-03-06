/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Common.h"
#include "TreeItemChildrenSerializer.h"
#include "Serializable.h"
#include "TreeItem.h"
#include "ProjectTreeItem.h"
#include "LayerGroupTreeItem.h"
#include "PianoLayerTreeItem.h"
#include "AutomationLayerTreeItem.h"
#include "InstrumentsRootTreeItem.h"
#include "InstrumentTreeItem.h"
#include "VersionControlTreeItem.h"
#include "SettingsTreeItem.h"

void TreeItemChildrenSerializer::serializeChildren(const TreeItem &parentItem, XmlElement &parentXml)
{
    for (int i = 0; i < parentItem.getNumSubItems(); ++i)
    {
        if (TreeViewItem *sub = parentItem.getSubItem(i))
        {
            TreeItem *treeItem = static_cast<TreeItem *>(sub);
            parentXml.addChildElement(treeItem->serialize());
        }
    }
}

void TreeItemChildrenSerializer::deserializeChildren(TreeItem &parentItem, const XmlElement &parentXml)
{
    forEachXmlChildElementWithTagName(parentXml, e, Serialization::Core::treeItem)
    {
        const String type = e->getStringAttribute("type");
        TreeItem *child = nullptr;

        if (type == Serialization::Core::project)
        {
            child = new ProjectTreeItem("");
        }
        else if (type == Serialization::Core::settings)
        {
            child = new SettingsTreeItem();
        }
        else if (type == Serialization::Core::layerGroup)
        {
            child = new LayerGroupTreeItem("");
        }
        else if (type == Serialization::Core::pianoLayer)
        {
            child = new PianoLayerTreeItem( "");
        }
        else if (type == Serialization::Core::autoLayer)
        {
            child = new AutomationLayerTreeItem("");
        }
        else if (type == Serialization::Core::instrumentRoot)
        {
            child = new InstrumentsRootTreeItem();
        }
        else if (type == Serialization::Core::instrument)
        {
            child = new InstrumentTreeItem();
        }
        else if (type == Serialization::Core::versionControl)
        {
            child = new VersionControlTreeItem();
        }

        if (child != nullptr)
        {
            parentItem.addChildTreeItem(child);
            child->deserialize(*e);
        }
    }

    // todo. загрузка долгая и стремная. рассылаются кучи событий,
    // перебилдится кэш миди кучу раз. чо за фигня, чувак.

    // and we need to go deeper.
}
