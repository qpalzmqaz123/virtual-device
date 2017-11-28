'use strict'

const BaseDevice = require('./base')

class Flowmeter extends BaseDevice {
  async init () {
    this.clear()
  }

  async read () {
    let buffer = Buffer.alloc(4)
    buffer.writeUInt32LE(this._flow)

    return Buffer.concat([ this.constructor.resTable.success, buffer ])
  }

  async clear () {
    this._flow = 0
  }

  async setAlarm (flow) {
    console.log(flow)

    return new Promise(resolve => {
      setTimeout(resolve, 1000)
    })
  }

  async received (data) {
    let opCode = data.readUInt8(0)
    let res

    switch (this.constructor.cmdTable[opCode]) {
      case 'init':
        this.init()
        break
      case 'read':
        res = await this.read()
        break
      case 'clear':
        res = await this.clear()
        break
      case 'setAlarm':
        res = await this.setAlarm(data.readUInt32LE(1))
        break
      default:
        break
    }

    return res || this.constructor.resTable.success
  }

  static get cmdTable () {
    return {
      0: 'init',
      1: 'read',
      2: 'clear',
      3: 'setAlarm'
    }
  }

  static get resTable () {
    return {
      'success': Buffer.from('\x00')
    }
  }
}

module.exports = Flowmeter
