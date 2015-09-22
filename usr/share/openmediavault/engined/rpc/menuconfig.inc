<?php
/**
 * @license     http://www.gnu.org/licenses/gpl.html GPL Version 3
 * @author      Ian Moore <imooreyahoo@gmail.com>
 * @author      Marcel Beck <marcel.beck@mbeck.org>
 * @author      OpenMediaVault Plugin Developers <plugins@omv-extras.org>
 * @author      Simon Baur <sbausis@gmx.net>
 * @copyright   Copyright (c) 2011 Ian Moore
 * @copyright   Copyright (c) 2012 Marcel Beck
 * @copyright   Copyright (c) 2009-2013 Volker Theile
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
require_once("openmediavault/object.inc");
require_once("openmediavault/config.inc");
require_once("openmediavault/error.inc");
require_once("openmediavault/util.inc");
require_once("openmediavault/rpcservice.inc");
require_once("openmediavault/notify.inc");

class OMVRpcServiceMenuconfig extends OMVRpcServiceAbstract {

    /**
     * Get the RPC service name.
     */
    public function getName() {
        return "Menuconfig";
    }

    /**
     * Initialize the RPC service.
     */
    public function initialize() {
        $this->registerMethod("getSettings");
        $this->registerMethod("setSettings");
		$this->registerMethod("getList");
		$this->registerMethod("getNodes");
		$this->registerMethod("setNodes");
		$this->registerMethod("get");
    }

    /**
     * Get the settings configuration object.
     * @param params The method parameters.
     * @param context The context of the caller.
     * @return The requested configuration object.
     * @throw E_CONFIG_GET_OBJECT_FAILED
     */
    public function getSettings($params, $context) {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, array( "role" => OMV_ROLE_ADMINISTRATOR ));

        // Get the configuration object.
        $xpath = "//services/menuconfig";
        $object = $xmlConfig->get($xpath);
        if(is_null($object))
            throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED, $xpath);

        // Modify the result object.
        $object['enable']          = boolval($object['enable']);

        // Remove useless fields from the result object.
        unset($object['entries']);

        return $object;
    }

    /**
     * Set the settings configuration object.
     * @param params The method parameters.
     * @param context The context of the caller.
     * @return The stored configuration object.
     * @throw E_CONFIG_GET_OBJECT_FAILED
     * @throw E_CONFIG_SET_OBJECT_FAILED
     */
    public function setSettings($params, $context) {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, array( "role" => OMV_ROLE_ADMINISTRATOR ));

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, '{
            "type"       : "object",
            "properties" : {
                "enable"             : { "type" : "boolean" }
            }
        }');

        // Get the existing configuration object.
        $xpath = "//services/menuconfig";
        $oldObject = $xmlConfig->get($xpath);
        if(is_null($oldObject))
            throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED, $xpath);

        // Update the configuration object.
        $object = array(
            "enable" 	=> array_boolval($params, 'enable'),
            "entries" 	=> isset($oldObject['entries']) ? $oldObject['entries'] : array()
        );

        if(FALSE === $xmlConfig->replace("//services/menuconfig", $object))
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);

        // Notify configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, "org.openmediavault.services.menuconfig", $object);

        // Return the configuration object.
        return $object;
    }

    /**
     * Get list of Entries
     *
     * @param $start     integer start point in paging list
     * @param $count     integer number of objects to return in paged list
     * @param $sortField string field to sort on
     * @param $sortDir   integer sort direction
     *
     * @return array list of Entries
     */
    public function getList($params, $context) {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, array( "role" => OMV_ROLE_ADMINISTRATOR ));

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, '{
            "type"       : "object",
            "properties" : {
                "start"     : { "type" : "integer" },
                "limit"     : { '.$GLOBALS['OMV_JSONSCHEMA_COUNTFIELD'].' },
                "sortfield" : { '.$GLOBALS['OMV_JSONSCHEMA_SORTFIELD'].' },
                "sortdir"   : { '.$GLOBALS['OMV_JSONSCHEMA_SORTDIR'].' }
            }
        }');

		$command = "/usr/local/sbin/omv-menunodes -s admin -j -l";
		$output = system($command, $ret);
        if(0 !== $ret)
            throw new OMVException(OMVErrorMsg::E_EXEC_FAILED, $command, implode("\n", $output));
		
		$node_list = json_decode($output, true);
		if ($node_list == NULL) {
			throw new OMVException(OMVErrorMsg::E_MISC_FAILURE, $node_list, implode("\n", $output));
		}
		
		$objects = array();
		for ($i = 0; $i < count($node_list); $i++) {
			array_push($objects, array("id" => $node_list[$i]["id"], "path" => $node_list[$i]["path"], ));
		}
		
        // Filter the result.
        return $this->applyFilter($objects, $params['start'],
            $params['limit'], $params['sortfield'], $params['sortdir']);
    }
	
	public function read_nodes($force) {
		$command = "/usr/local/sbin/omv-menunodes -s admin -j -l";
		if ($force == true) $command = " -c";
		
		$output = system($command, $ret);
        if(0 !== $ret)
            throw new OMVException(OMVErrorMsg::E_EXEC_FAILED, $command, implode("\n", $output));
		
		$node_list = json_decode($output, true);
		if ($node_list == NULL) {
			throw new OMVException(OMVErrorMsg::E_MISC_FAILURE, $node_list, implode("\n", $output));
		}
		
		$objects = array();
		for ($i = 0; $i < count($node_list); $i++) {
			array_push($objects, array("id" => $node_list[$i]["id"], "path" => $node_list[$i]["path"], ));
		}
		return $objects;
	}
	
	public function getNodes($params, $context) {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, array( "role" => OMV_ROLE_ADMINISTRATOR ));

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, '{
            "type"       : "object",
            "properties" : {
                "start"     : { "type" : "integer" },
                "limit"     : { '.$GLOBALS['OMV_JSONSCHEMA_COUNTFIELD'].' },
                "sortfield" : { '.$GLOBALS['OMV_JSONSCHEMA_SORTFIELD'].' },
                "sortdir"   : { '.$GLOBALS['OMV_JSONSCHEMA_SORTDIR'].' }
            }
        }');
		
		$objects = $this->read_nodes(true)
		
        // Filter the result.
        return $this->applyFilter($objects, $params['start'],
            $params['limit'], $params['sortfield'], $params['sortdir']);
    }
	
	public function setNodes($params, $context) {
		global $xmlConfig;
		// Validate the RPC caller context.
		$this->validateMethodContext($context, array(
			  "role" => OMV_ROLE_ADMINISTRATOR
		  ));
		// Validate the parameters of the RPC service method.
		$this->validateMethodParams($params, '{
			  "type":"array",
			  "items":{
				  "type":"object",
				  "properties":{
					  "id":{"type":"string"},
					  "enable":{"type":"boolean"}
				  }
			  }
		  }');
		// Prepare the configuration object.
		$object = array();
		// Get the list of registered modules.
		$moduleMgr = &OMVModuleMgr::getInstance();
		$modules = $moduleMgr->getModules();
		foreach($modules as $modulev) {
			// Check if the module implements the OMVIModuleNotification
			// interface.
			if($modulev instanceof OMVIModuleNotification) {
				$config = $modulev->getNotificationConfig();
				foreach($config as $configk => $configv) {
					$enable = FALSE; // Default value.
					foreach($params as $paramk => $paramv) {
						if($paramv['id'] === $configv['id']) {
							$enable = array_boolval($paramv, 'enable');
							break;
						}
					}
					$object[$configv['id']] = $enable;
				}
			}
		}
		// Set the configuration object.
		$xpath = "//system/notification/notifications";
		if(FALSE === $xmlConfig->replace($xpath, $object)) {
			throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
		}
		// Notify the configuration changes.
		$dispatcher = &OMVNotifyDispatcher::getInstance();
		$dispatcher->notify(OMV_NOTIFY_MODIFY,
		  "org.openmediavault.system.notification.notifications", $object);
		// Return the configuration object.
		return $object;
	}

	public function get($params, $context) {
		global $xmlConfig;
		// Validate the RPC caller context.
		$this->validateMethodContext($context, array(
			  "role" => OMV_ROLE_ADMINISTRATOR
		  ));
		$result = array();
		// Get the configuration object.
		$xpath = "//system/notification/notifications";
		$object = $xmlConfig->get($xpath);
		if(is_null($object)) {
			throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
			  $xpath);
		}
		// Get the list of registered modules.
		$moduleMgr = &OMVModuleMgr::getInstance();
		$modules = $moduleMgr->getModules();
		foreach($modules as $modulev) {
			// Check if the module implements the OMVIModuleNotification
			// interface.
			if($modulev instanceof OMVIModuleNotification) {
				$config = $modulev->getNotificationConfig();
				foreach($config as $configk => $configv) {
					$enable = FALSE; // Default value.
					if(array_key_exists($configv['id'], $object)) {
						$enable = boolvalEx($object[$configv['id']]);
					}
					$result[] = array(
						  "id" => $configv['id'],
						  "type" => $configv['type'],
						  "title" => $configv['title'],
						  "enable" => $enable
					  );
				}
			}
		}
		return $result;
	}
	
}

// Register the RPC service.
$rpcServiceMgr = &OMVRpcServiceMgr::getInstance();
$rpcServiceMgr->registerService(new OMVRpcServiceMenuconfig());
