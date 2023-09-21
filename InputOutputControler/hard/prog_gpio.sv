module  prog_gpio(
  input  clk,
  input  reset_n,
  input  avs_write,
  input  [31:0] avs_writedata,
  input  [4:0]  avs_address,
  input  [31:0] pio_i,
  output [31:0] avs_readdata,
  output [31:0] pio_o,
  output irq

);

logic[31:0] out;

// Register data out
logic[31:0] data_out;

// Register data out
logic[31:0] data_in;

// Register enable
logic[31:0] enable;

// Register interruption request mask
logic[31:0] irq_mask;

// Register interruption request policy
logic[31:0] irq_pol;

// Register interruption request acknowledge
logic irq_ack;

always_ff @(posedge clk or negedge reset_n)
  if(!reset_n)
    begin
    data_out <= '0;
    enable <= '0;
    irq_mask <= '0;
    irq_pol <= '0;
    irq_ack <= '0;
    end
  else
    begin
    irq_ack <= '0;
    if(avs_write)
    case (avs_address[4:0])
      0 : data_out <= avs_writedata & enable;
      4 : enable <= avs_writedata;
      8 : irq_mask <= avs_writedata;
      12 : irq_pol <= avs_writedata;
      16 : irq_ack <= '1;
    endcase
    end
    

always_ff @(posedge clk or negedge reset_n)
if(!reset_n)
  data_in <= '0;
else
  data_in <= pio_i;

always_comb
begin
   case (avs_address[4:0])
    0 : out = data_in & enable;
    4 : out = enable;
    8 : out = irq_mask;
    12 : out = irq_pol;
    16 : out = '0;
    default : out = '0;
   endcase
end


wire irq_sig = | ( (data_in ^ irq_pol) & enable & irq_mask );

logic irq_reg;
always_ff @(posedge clk or negedge reset_n)
  if(!reset_n)
    irq_reg <= '0;
  else
    if(irq_ack)
      irq_reg <= '0;
    else if(irq_sig)
      irq_reg <= '1;



assign avs_readdata = out;
assign irq = irq_reg;
assign pio_o  = data_out;





endmodule
