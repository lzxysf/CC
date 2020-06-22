/**
 * 
 * 推荐使用let，不使用var，let和C语言等定义变量是类似的
 * 
 * var太灵活，可以重复定义，如
 * var x = 1;
 * var x = 2;
 * 是可以通过的，而let就会报错。
 * 另外关键的是用var定义的变量在不同作用域之间也会相互影响
 * 
 * let是更完美的var
 * let声明的变量拥有块级作用域。 也就是说用let声明的变量的作用域只是外层块，而不是整个外层函数
 * 
 */

function varTest() {
    var x = 1;
    if (true) {
      var x = 2;  // same variable!
      console.log(x);  // 2
    }
    console.log(x);  // 2
  }
  
  function letTest() {
    let x = 1;
    if (true) {
      let x = 2;  // different variable
      console.log(x);  // 2
    }
    console.log(x);  // 1
  }

  varTest();
  letTest();