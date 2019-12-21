Page({
  data: {
    a: [],
  },
  add: function () {
    var a_ = this.data.a
    a_.push(' ')
    this.setData({
      a: a_
    })
    console.log(this.data.a);
  },
  delete: function () {
    var a_ = this.data.a
    console.log(a_)
    a_.splice(0, 1)
    this.setData({
      a: a_
    })


  },
  //时间选择器
  bindTimeChange: function (e) {
    console.log('picker发送选择改变，携带值为', e.detail.value)
    this.setData({
      time: e.detail.value
    })
  },
  formSubmit: function (e) {
    var times = e.detail.value.times
    var intervals = e.detail.value.intervals
    console.log(e)
    wx.cloud.callFunction({
      name: 'login',
      data: {
        times: times,
        intervals: intervals,
      },
      success: function (res) {
        console.log(res)
      },
      restart: function (e) {
      }
    })
  },
  button: function () {
    wx.showToast({
      title: '设定成功',
      icon: 'success'
    })
  }
})
