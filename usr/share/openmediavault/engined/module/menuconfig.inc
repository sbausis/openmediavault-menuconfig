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
require_once("openmediavault/module.inc");
#require_once("openmediavault/config.inc");
require_once("openmediavault/error.inc");
#require_once("openmediavault/util.inc");
#require_once("openmediavault/initscript.inc");
#require_once("openmediavault/log.inc");

class OMVModuleMenuconfig extends OMVModuleServiceAbstract
    implements OMVINotifyListener, OMVIModuleServiceStatus {

    /**
     * Get the module name.
     */
    public function getName() {
        return "menuconfig";
    }

    public function applyConfig() {
        global $xmlConfig;

        $cmd = "omv-mkconf menuconfig 2>&1";
        if(0 !== $this->exec($cmd, $output))
            throw new OMVException(OMVErrorMsg::E_EXEC_FAILED, $cmd, implode("\n", $output));

        // Restart samba enabling dnsmasq or enabling dns-wins - not sure how
        //$xpath = "//services/dnsmasq";
        //$object = $xmlConfig->get($xpath);
        //if(is_null($object))
        //    throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED, $xpath);
        //
        //if(FALSE === boolval($object['enable']))
        //    return;
        //
        //$initd = new OMVSysVInitScript("dnsmasq", TRUE);
        //$initd->restart();
    }

    /**
     * Bind listeners.
     */
    function bindListeners(OMVNotifyDispatcher $dispatcher) {
        /* Update service if configuration has been modified */
        $dispatcher->addListener(
            OMV_NOTIFY_MODIFY,
            "org.openmediavault.services.menuconfig",
            array($this, "setDirty"), 1);
    }

}

// Register module
$moduleMgr = &OMVModuleMgr::getInstance();
$moduleMgr->registerModule(new OMVModuleMenuconfig());

?>
