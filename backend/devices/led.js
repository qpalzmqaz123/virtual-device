'use strict'

const BaseDevice = require('./base')

class LED extends BaseDevice {
  async received (data) {
    let opCode = data.readUInt8()

    switch (LED.cmdTable[opCode]) {
      case 'init':
        this._isOn = false
        break
      case 'on':
        this._isOn = true
      case 'off':
        this._isOn = false
      case 'toggle':
        this._isOn = !this._isOn
      default:
        break
    }

    console.log(`LED${this.deviceId}: ${this._isOn ? 'ON' : 'OFF'}`)

    return LED.resTable.success
  }

  static get cmdTable () {
    return {
      0: 'init',
      1: 'on',
      2: 'off',
      3: 'toggle'
    }
  }

  static get resTable () {
    return {
      'success': Buffer.from('\x00\x00\x00\x00')
    }
  }
}

module.exports = LED
