require "#{File.dirname(__FILE__)}/../spec_helper.rb"

include V8

describe C::Function do
  it "is callable" do
    Context.new do |cxt|
      f = cxt.eval('(function() {return "Hello World"})', '<eval>');      
      f.call().should == "Hello World"      
    end
  end
  
  it "receives proper argument length from ruby" do
    Context.new do |cxt|
      f = cxt.eval('(function() {return arguments.length})', 'eval')
      f.call(1, 2, 3).should == 3
    end
  end
  
  it "maps all arguments from ruby" do
    Context.new do |cxt|
      f = cxt.eval('(function(one, two, three) {return one + two + three})', 'eval')
      f.call(1,2,3).should == 6
    end
  end
  
  it "properly maps ruby objects back and forth from arguments to return value" do
    Context.new do |cxt|
      Object.new.tap do |this|
         f = cxt.eval('(function() {return this})', 'eval')
         f.methodcall(this).should be(this)
      end
    end
  end 
  
  it "can be called outside of a context" do
    Context.new do |cxt|
      @f = cxt.eval('(function() {return "Call Me"})', 'eval')
    end
    @f.call().should == "Call Me"
  end
  
  it "is reflected properly" do
    Context.new do |cxt|
      cxt['say'] = lambda {|word, times| word * times}
      cxt.eval('say("Hello", 3)').should == "HelloHelloHello"
    end
  end
  
  it "has a name" do
    Context.new do |cxt|
      f = cxt.eval('(function hi() {return "Hello World"})', '<eval>')
      f.name.should == "hi"
    end
  end

  it "can have its name set" do
    Context.new do |cxt|
      f = cxt.eval('(function () {return "Goodbye World"})', '<eval>')
      f.name = 'bye'
      f.name.should == 'bye'
    end
  end

  it "should be able to timeout" do
    lambda {
      Context.new do |cxt|
        cxt.timeout = 100
        cxt.eval 'while(true) {}'
      end
    }.should raise_error(Timeout::Error)
  end

  it "should be able to create another context from within a context" do
    def add_cxt(n1, n2)
      sum = nil
      Context.new do |cxt|
        cxt['sum'] = lambda { |a, b| a + b }
        sum = cxt.eval("sum(#{n1}, #{n2});")
      end
      sum
    end

    Context.new do |cxt|
      cxt['add'] = lambda { |a, b| add_cxt a, b }
      cxt.eval("add(1,1);").should == 2
    end
  end

  it "should timeout even when a 2nd context takes to long" do
    def do_loop
      Context.new do |cxt|
        cxt.eval 'while(true) {}'
      end
    end

    lambda {
      Context.new do |cxt|
        cxt['loop'] = lambda { do_loop }
        cxt.timeout = 10000
        cxt.eval("loop();");
      end
    }.should raise_error(Timeout::Error)
  end

  it "should error even when the 2nd context times out" do
    def do_loop
      Context.new do |cxt|
        cxt.timeout = 100
        cxt.eval 'while(true) {}'
      end
    end

    lambda {
      Context.new do |cxt|
        cxt['loop'] = lambda { do_loop }
        cxt.eval("loop();");
      end
    }.should raise_error(V8::JSError)
  end

#  Below test can seg fault. Sometimes the test will pass.
#  Not sure if it is a just an issue with Ruby Threads
#  it "should work with multiple threads" do
#    locked = true
#
#    cxt1 = Context.new do |cxt|
#      cxt['locked'] = lambda { locked }
#    end
#
#    cxt2 = Context.new do |cxt|
#      cxt['unlock'] = lambda { locked = false }
#    end
#
#    t1 = Thread.new do
#      cxt1.eval("while(locked()) {}")
#    end
#
#    t2 = Thread.new do
#      cxt2.eval("unlock();")
#    end
#
#    t2.join
#    t1.join
#  end
end
