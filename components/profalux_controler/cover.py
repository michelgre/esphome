import esphome.codegen as cg
import esphome.config_validation as cv

from esphome import automation
from esphome.components import cover

from esphome.const import (
    CONF_ID,
)

from . import ProfaluxControler

DEPENDENCIES = ['profalux_controler']

# from esphome.components.profalux_blind.output_pin import OutputPin

profalux_controler_ns = cg.esphome_ns.namespace("profalux_controler")

ProfaluxBlind = profalux_controler_ns.class_("ProfaluxBlind", cover.Cover, cg.Component)

CONF_PROFALUX_BLIND_ID = "conf_profalux_blind_id"
CONF_CONTROLER_ID = "controler_id"
CONF_BLIND_NUMBER = "blind_number"

CONFIG_SCHEMA = cover.COVER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ProfaluxBlind),
    cv.GenerateID(CONF_CONTROLER_ID): cv.use_id(ProfaluxControler),
    cv.Required(CONF_BLIND_NUMBER): cv.positive_int,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    paren = yield cg.get_variable(config[CONF_CONTROLER_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield cover.register_cover(var, config)
    cg.add(paren.register_blind(var))
    cg.add(var.setNumber(config[CONF_BLIND_NUMBER]))
    