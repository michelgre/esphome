import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.const import CONF_ENTITY_ID, CONF_NAME, CONF_ID, CONF_INTERNAL, CONF_DISABLED_BY_DEFAULT
from esphome.const import (
    CONF_ID,
)

profalux_controler_ns = cg.esphome_ns.namespace("profalux_controler")

ProfaluxControler = profalux_controler_ns.class_("ProfaluxControler", cg.Component)

# CONF_PROFALUX_CONTROLER_ID = "controler_id"
CONF_PINLIST = "pin-list"
CONF_CTRL_NAME = "controler-name"
CONF_SIGNAL_DURATION = "signal-duration"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ProfaluxControler),
    cv.Optional(CONF_PINLIST): cv.string,
    cv.Optional(CONF_CTRL_NAME): cv.string,
    cv.Optional(CONF_SIGNAL_DURATION): cv.positive_int,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)    
    cg.add(var.set_controler_id(config[CONF_CTRL_NAME]))
    cg.add(var.set_signal_duration(config[CONF_SIGNAL_DURATION]))

