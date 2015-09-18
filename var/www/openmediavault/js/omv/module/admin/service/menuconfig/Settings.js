/**
 * @license     http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author      Ian Moore <imooreyahoo@gmail.com>
 * @author      Marcel Beck <marcel.beck@mbeck.org>
 * @author      OpenMediaVault Plugin Developers <plugins@omv-extras.org>
 * @author      Simon Baur <sbausis@gmx.net>
 * @copyright   Copyright (c) 2011 Ian Moore
 * @copyright   Copyright (c) 2012 Marcel Beck
 * @copyright   Copyright (c) 2013-2014 OpenMediaVault Plugin Developers
 * @copyright   Copyright (c) 2015 Simon Baur
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
// require("js/omv/WorkspaceManager.js")
// require("js/omv/workspace/form/Panel.js")
// require("js/omv/workspace/window/Form.js")
// require("js/omv/data/Store.js")
// require("js/omv/data/Model.js")
// require("js/omv/data/proxy/Rpc.js")
// require("js/omv/workspace/window/plugin/ConfigObject.js")

Ext.define("OMV.module.admin.service.menuconfig.Settings", {
    extend : "OMV.workspace.form.Panel",

    /*plugins : [{
        ptype        : "linkedfields",
        correlations : [{
            name       : [
                "gateway"
            ],
            conditions : [{
                name  : "dhcp-enable",
                value : false
            }],
            properties : "allowBlank"
        }]
    }],*/

    initComponent : function () {
        var me = this;

        me.on('load', function () {
            var checked = me.findField('enable').checked;
            var parent = me.up('tabpanel');

            if (!parent)
                return;

            var entriesPanel = parent.down('panel[title=' + _("Entries") + ']');

            if (entriesPanel) {
                checked ? entriesPanel.enable() : entriesPanel.disable();
            }
        });

        me.callParent(arguments);
    },

    rpcService   : "Menuconfig",
    rpcGetMethod : "getSettings",
    rpcSetMethod : "setSettings",

    getFormItems: function () {
        return [{
            xtype    : "fieldset",
            title    : _("General"),
            defaults : {
                labelSeparator : ""
            },
            items    : [{
                xtype      : "checkbox",
                name       : "enable",
                fieldLabel : _("Enable"),
                checked    : false
            }]
        }];
    }
});

OMV.WorkspaceManager.registerPanel({
    id        : "settings",
    path      : "/service/menuconfig",
    text      : _("Settings"),
    position  : 10,
    className : "OMV.module.admin.service.menuconfig.Settings"
});
