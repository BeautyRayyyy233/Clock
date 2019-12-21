Page({
  data: {
    userName: '',
    pasWInput: '',
  
  },
  datePickerBindchange: function (e) {
    this.setData({
      dateValue: e.detail.value
    })
  },
  onLoad: function (options) {
    // 页面初始化 options为页面跳转所带来的参数
  },
  onReady: function () {
    // 页面渲染完成
  },
  onShow: function () {
    // 页面显示
  },
  onHide: function () {
    // 页面隐藏
  },
  onUnload: function () {
    // 页面关闭
  },
  imgClick: function () {
  },
  // 用户名和密码输入框事件
  userNameInput: function (e) {
    this.setData({
      userName: e.detail.value
    })
  },
  pasWInput: function (e) {
    this.setData({
      SFZ: e.detail.value
    })
  },

  bindFormSubmit: function (e) {
    if (e.detail.value.userName.length == 0 || e.detail.value.pasWInput.length == 0) {
      this.setData({
        tip: '提示：用户信息！',
        userName: '',
        psw: ''
      })
    } else {
    }
  },
})
