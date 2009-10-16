require File.dirname(__FILE__) + '/spec_helper.rb'

describe "The Ruby Racer" do
  
  before(:each) do
    @cxt = V8::Context.new
  end
  
  describe  "Type Conversion from Ruby to Javascript" do
    
    it "can pass nil back to ruby" do
      eval("null").should be_nil      
    end
    
    it "passes back undefined value as nil" do
      pending "This currently causes a segmentation fault..."
      eval("this.undefined").should be_nil
    end
    
    it "can pass strings back to ruby" do
      eval("'Hello World'").should == "Hello World"
    end
    
    it "can pass doubles back to ruby" do
      eval("2.5").should == 2.5
    end
    
    it "can pass fixed numbers back to ruby" do
      eval("1").should == 1
    end
    
    it "can pass boolean values back to ruby" do
      eval("true").should be(true)
      eval("false").should be(false)
    end
        
  end
  
  def eval(str)
    @cxt.eval(str)
  end
  
end