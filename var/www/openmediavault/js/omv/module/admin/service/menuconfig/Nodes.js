
// require("js/omv/WorkspaceManager.js")
// require("js/omv/Rpc.js")
// require("js/omv/data/Store.js")
// require("js/omv/data/Model.js")
// require("js/omv/data/proxy/Rpc.js")
// require("js/omv/window/MessageBox.js")
// require("js/omv/workspace/form/Panel.js")
// require("js/omv/workspace/grid/Panel.js")
// require("js/omv/form/field/Password.js")
// require("js/omv/util/Format.js")

Ext.define("OMV.module.admin.service.menuconfig.Nodes", {
    extend   : "OMV.workspace.grid.Panel",
    requires : [
        "OMV.Rpc",
        "OMV.data.Store",
        "OMV.data.Model",
        "OMV.data.proxy.Rpc"
    ],

    hidePagingToolbar : true,
    hideAddButton     : true,
    hideEditButton    : true,
    hideDeleteButton  : true,
    stateful          : true,
    stateId           : "9876057b-b2c0-4c48-a4c1-8c9b4fb54d7b",
    columns           : [{
        text      : _("fullpath"),
        sortable  : true,
        dataIndex : "fullpath",
        stateId   : "fullpath"
    },{
        text      : _("id"),
        sortable  : true,
        dataIndex : "id",
        stateId   : "id"
    },{
        text      : _("path"),
        sortable  : true,
        dataIndex : "path",
        stateId   : "path"
    },{
        text      : _("enable"),
        sortable  : true,
        dataIndex : "enable",
        stateId   : "enable"
    }],

    initComponent : function () {
        var me = this;
        Ext.apply(me, {
            store : Ext.create("OMV.data.Store", {
                autoLoad : true,
                model    : OMV.data.Model.createImplicit({
                    idProperty  : "fullpath",
                    fields      : [
						{ name: "id", type: "string" },
						{ name: "path", type: "string" },
						{ name: "fullpath", type: "string" },
						{ name: "enable", type: "boolean" }
                    ]
                }),
                proxy : {
                    type    : "rpc",
					appendSortParams: false,
                    rpcData : {
                        service : "Menuconfig",
                        method  : "getNodes"
                    }
                }
            })
        });
        me.callParent(arguments);
    }
});

OMV.WorkspaceManager.registerPanel({
    id        : "nodes",
    path      : "/service/menuconfig",
    text      : _("Nodes"),
    position  : 30,
    className : "OMV.module.admin.service.menuconfig.Nodes"
});
