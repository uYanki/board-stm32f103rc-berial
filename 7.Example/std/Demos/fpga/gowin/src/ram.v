module ram (
        input cs_i,
        input sclk,
        input din,
        output reg dout
    );
    
    reg [ 2: 0 ] bit;
    
    reg [ 7: 0 ] addr, data;
    
//    localparam STATE_IDEL = 5'b00001;
//    localparam STATE_RXMODE = 5'b00010;
//    localparam STATE_RXADDR = 5'b00100;
//    localparam STATE_RXDATA = 5'b01000;
//    localparam STATE_TXDATA = 5'b10000;
    localparam STATE_IDEL = 5'd0;
    localparam STATE_RXMODE = 5'd1;
    localparam STATE_RXADDR = 5'd2;
    localparam STATE_RXDATA = 5'd3;
    localparam STATE_TXDATA = 5'd4;
    reg [ 4: 0 ] cstate, nstate;
    
    localparam MODE_READ = 1'b0;
    localparam MODE_WRITE = 1'b1;
    reg mode;
    
    // 逻辑输出
    
    wire [ 7: 0 ] ramout;
    
    Gowin_SP ram_inst(
                 .dout( ramout ),
                 .clk( sclk ),
                 .oce( 1'b1 ),
                 .ce( 1'b1 ),
                 .reset( 1'b0 ),
                 .wre( mode ),
                 .ad( addr ),
                 .din( data )
             );
    
    // 状态转移
    
    always @(*) begin
        case ( cstate )
            STATE_IDEL: begin
                nstate = STATE_RXMODE;
            end
            STATE_RXMODE: begin
                nstate = STATE_RXADDR;
            end
            STATE_RXADDR: begin
                if ( bit == 3'd7 ) begin
                    case ( mode )
                        MODE_READ: nstate = STATE_TXDATA;
                        MODE_WRITE: nstate = STATE_RXDATA;
                    endcase
                end
            end
            STATE_RXDATA, STATE_TXDATA: begin

            end
        endcase
    end

    always @(posedge sclk or posedge cs_i) begin
         cstate <= cs_i ? STATE_IDEL:nstate;
    end
    
    // 逻辑输入输出
    
    always @( posedge sclk ) begin
        // first bit -> msb
        case ( cstate )
            STATE_IDEL : begin
                mode <= 1'b0;
                addr <= 8'd0;
                data <= 8'd0;
                bit <= 3'd0 ;
            end
            STATE_RXMODE: begin
                mode <= din;
            end
            STATE_RXADDR: begin
                addr[ 0 ] <= din;
                addr <= addr << 1;
            end
            STATE_RXDATA: begin
                data[ 0 ] <= din;
                data <= data << 1 ;
            end
            STATE_TXDATA: begin
                data <= ( bit == 3'd0 ) ? ( ramout ) : ( data >> 1 );
                dout <= data[ 0 ];
            end
            STATE_RXADDR,STATE_RXDATA,STATE_TXDATA: begin
                if( bit == 3'd7 ) begin
                    bit <= 3'd0 ;
                    addr <= addr + 1'b1;
                end else begin
                    bit <= bit + 1'b1;
                end
           
            end
        endcase
    end
    
endmodule
