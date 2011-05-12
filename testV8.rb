#!/usr/bin/env ruby 
require 'rubygems' 
$:.unshift(File.dirname(__FILE__) + '/lib') 
$:.unshift(File.dirname(__FILE__) + '/contrib') 
require 'v8' 
include V8 
# V8::Context.constraints(256, 10000)
begin 
Context.new do |cxt| 
  
  cxt.timeout = 2000000 # 100 ms timeout 
  begin 
    cxt["say"] = lambda {|word| print( word )} 
    cxt.eval('var i = 0; var arr = Array(); for( var j=0; j < 10 ; ++j ){ say("."); }' )
    puts "here" 
  rescue Exception => e 
    puts e 
  end
  
  begin
    cxt.eval('var i = 0; var arr = Array(); for( var j=0; j < 10000000000000000  ; ++j ){ /*say("."); */}' ) 
    puts "here"
  rescue Exception => e 
    puts e
  end
  
  begin
    cxt.eval('ddddvar i = 0; var arr = Array(); for( var j=0; j < 10000000000000000  ; ++j ){ /*say("."); */}' ) 
    puts "here"
  rescue Exception => e 
    puts e
  end
  
  begin
    cxt.eval('var i = 0; var arr = Array(); for( var j=0; j < 10000000000000000  ; ++j ){ alert("j"); }' ) 
    puts "here"
  rescue Exception => e 
    puts e
  end

end
  
end 
