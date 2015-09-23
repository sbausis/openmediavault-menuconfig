
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
                    idProperty  : "id",
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

//Ext.define("OMV.module.admin.service.menuconfig.Nodes", {
//	extend: "OMV.workspace.grid.Panel",
//	requires: [
//		"OMV.data.Store",
//		"OMV.data.Model",
//		"OMV.data.proxy.Rpc",
//		"OMV.util.Format"
//	],
//
//	stateful: true,
//	stateId: "e03bdc0e-4f66-11e2-8510-00221568ca88",
//	bodyCls: "x-grid-without-dirty-cell",
//	features: [{
//		ftype: "grouping",
//		groupHeaderTpl: "{name}"
//	}],
//	columns: [{
//		text: _("fullpath"),
//		sortable: true,
//		groupable: true,
//		hidden: false,
//		dataIndex: "fullpath",
//		stateId: "fullpath",
//		align: "center",
//		renderer: function(value, metaData, record, rowIndex, colIndex,
//		  store, view) {
//			return _(value);
//		}
//	},{
//		text: _("id"),
//		sortable: true,
//		groupable: false,
//		dataIndex: "id",
//		stateId: "id",
//		flex: 1,
//		renderer: function(value, metaData, record, rowIndex, colIndex,
//		  store, view) {
//			return _(value);
//		}
//	},{
//		text: _("path"),
//		sortable: true,
//		groupable: false,
//		dataIndex: "path",
//		stateId: "path",
//		flex: 1,
//		renderer: function(value, metaData, record, rowIndex, colIndex,
//		  store, view) {
//			return _(value);
//		}
//	},{
//		xtype: "checkcolumn",
//		text: _("Enable"),
//		groupable: false,
//		dataIndex: "enable",
//		stateId: "enable",
//		align: "center",
//		width: 100,
//		resizable: false
//	}],
//	hideAddButton: true,
//	hideEditButton: true,
//	hideDeleteButton: true,
//	hideApplyButton: false,
//	hideRefreshButton: false,
//
//	initComponent: function() {
//		var me = this;
//		Ext.apply(me, {
//			store: Ext.create("OMV.data.Store", {
//				autoLoad: true,
//				groupField: "fullpath",
//				model: OMV.data.Model.createImplicit({
//					idProperty: "id",
//					fields: [
//						{ name: "id", type: "string" },
//						{ name: "path", type: "string" },
//						{ name: "fullpath", type: "string" },
//						{ name: "enable", type: "boolean" }
//					]
//				}),
//				proxy: {
//					type: "rpc",
//					appendSortParams: true,
//					rpcData: {
//						service: "Menuconfig",
//						method: "getNodes"
//					}
//				},
//				sorters: [{
//					direction: "ASC",
//					property: "id"
//				}]
//			})
//		});
//		me.callParent(arguments);
//	},
//
//	onApplyButton: function() {
//		var me = this;
//		var records = me.store.getRange();
//		var params = [];
//		//Ext.Array.each(records, function(record) {
//		//	params.push({
//		//		  "fullpath": record.get("fullpath"),
//		//		  "enable": record.get("enable")
//		//	  });
//		//});
//		//// Execute RPC.
//		//OMV.Rpc.request({
//		//	  scope: me,
//		//	  callback: function(id, success, response) {
//		//		  this.store.reload();
//		//	  },
//		//	  relayErrors: false,
//		//	  rpcData: {
//		//		  service: "Menuconfig",
//		//		  method: "setNodes",
//		//		  params: params
//		//	  }
//		//  });
//	}
//});

OMV.WorkspaceManager.registerPanel({
    id        : "nodes",
    path      : "/service/menuconfig",
    text      : _("Nodes"),
    position  : 30,
    className : "OMV.module.admin.service.menuconfig.Nodes"
});
