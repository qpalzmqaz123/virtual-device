'use strict'

const BaseDevice = require('./base')

class LED extends BaseDevice {
  async received (data) {
    let opCode = data.readUInt8()

    switch (LED.cmdTable[opCode]) {
      case 'init':
        console.log(`LED${this.deviceId} INIT`)
        break
      default:
        break
    }
  }

  static get cmdTable () {
    return {
      0: 'init',
      1: 'on',
      2: 'off',
      3: 'toggle'
    }
  }
}

module.exports = LED
