module  simple_mm_register(
  input  clk,
  input  reset_n,
  input  avs_write,
  input  [31:0] avs_writedata,
  output [31:0] avs_readdata,
  output [9:0]  leds

);

logic[31:0] R;

always_ff @(posedge clk or negedge reset_n)
  if(!reset_n)
    R <= '0;
  else if(avs_write)
    R <= avs_writedata;

assign avs_readdata = R;
assign leds  = R[9:0];


endmodule