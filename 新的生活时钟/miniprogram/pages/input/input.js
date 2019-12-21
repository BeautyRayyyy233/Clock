Page({
  data:{
    text:'',
    t_length:0
  },

  bindText: function (e) {
    var t_text = e.detail.value.length;
    var text = e.detail.value;
    this.setData({
      t_length: t_text,
      text: text
    })
  },
  button:function(){
    wx.showToast({
      title: '已记至小库',
      icon:'success',
      duration:2000
    })
  }
})