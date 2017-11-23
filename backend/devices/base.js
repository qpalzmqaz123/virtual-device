'use strict'

class BaseDevice {
  constructor (deviceModel, deviceId, writeStream) {
    this._deviceModel = deviceModel
    this._deviceId = deviceId
    this._writeStream = writeStream
  }

  get deviceId () {
    return this._deviceId
  }

  get deviceModel () {
    return this._deviceModel
  }

  async received () {
  }

  async send () {
  }
}

module.exports = BaseDevice
