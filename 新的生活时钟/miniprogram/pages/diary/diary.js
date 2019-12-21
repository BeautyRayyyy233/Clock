Page({
  data:{
    times:0
  },

  times:function(){
    var times = this.data.times
    this.setData({
      times:times + 1,
    })
  },


  saying:e=>{
    wx.navigateTo({
      url: '../input/input'
    })
  },
})