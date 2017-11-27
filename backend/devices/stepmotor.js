'use strict'

const BaseDevice = require('./base')

class Stepmotor extends BaseDevice {
  async received (data) {
    let opCode = data.readUInt8(0)

    switch (this.constructor.cmdTable[opCode]) {
      case 'init':
        this._angle = 0
        this._speed = 0
        break
      case 'setSpeed':
        this._speed = data.readUInt32LE(1)
        break
      case 'setAngle':
        this._angle = data.readFloatLE(1)
        break
      case 'step':
        break
      default:
        break
    }

    await this.update()

    return this.constructor.resTable.success
  }

  async update () {
    console.log(this._angle, this._speed)
  }

  static get cmdTable () {
    return {
      0: 'init',
      1: 'setSpeed',
      2: 'setAngle',
      3: 'step'
    }
  }

  static get resTable () {
    return {
      'success': Buffer.from('\x00')
    }
  }
}

module.exports = Stepmotor
