% Configure the Simulation with this Script (to add .txt config)
clear;
clc;

% Desired Number of Runs
Run_Count = 10;

% Re-Seed Noise Generators
new_seed = randi(1e4, 1);
    % RF
    set_param('LL_rdf2/Tag/Noise_1', 'Seed', num2str(new_seed))
    set_param('LL_rdf2/Tag/Noise_2', 'Seed', num2str(new_seed))
    % LO
    set_param('LL_rdf2/LO/Noise_I', 'Seed', num2str(new_seed))
    set_param('LL_rdf2/LO/Noise_Q', 'Seed', num2str(new_seed))


% Data visualization
data_vis = {'tag_angle', 'Bearing'};
keySet = {'tag_angle', 'Separation','RF_Noise_1', 'RF_Noise_2', 'RF_Noise', 'LO_Noise_1', 'LO_Noise_2', 'LO_Noise'};
valueSet = {{'LL_rdf2/Tag', 'off_angle_deg', 'tag_angle(j)'},...
            {'LL_rdf2/Tag', 'sep', 'TOI_ANT_Separation(j)'},...
            {'LL_rdf2/Tag', 'np_1','RF_Noise_1(j)'},...
            {'LL_rdf2/Tag', 'np_2','RF_Noise_2(j)'},...
            {'LL_rdf2/Tag', 'np_1','RF_Noise_1(j)', 'np_2','RF_Noise_2(j)'},...
            {'LL_rdf2/LOI', 'noise_i', 'LO_Noise_1(j)'},...
            {'LL_rdf2/LOI', 'noise_q', 'LO_Noise_2(j)'},...
            {'LL_rdf2/LOI', 'noise_i', 'LO_Noise_1(j)', 'noise_q', 'LO_Noise_2(j)'}};
            
paramMap = containers.Map(keySet, valueSet);

% Path set
path = paramMap(data_vis{1})

% Angle Range in Degrees
tag_angle = [0, 180];           

% Time span, seconds
stop_time = 15;

%TODO: FIGURE OUT WHAT THE UNITS OF THIS NOISE BLOCK ARE

% LO Noise Power, both I (1) and Q (2)
LO_Noise_1 = [1e-6 1e-3];
LO_Noise_2 = [1e-6 1e-3];

% Incoming Signal Noise Power, both antennas
RF_Noise_1 = [1e-6, 1e-3];
RF_Noise_2 = [1e-6, 1e-3];

% Separation of two antennas
TOI_ANT_Separation = [.45];

% Set up and run sim

set_param('LL_rdf2', 'StopTime', num2str(stop_time));
dim = 0;
if ~strcmp(data_vis{1}, 'tag_angle')
    set_param('LL_rdf2/Tag', 'off_angle_deg', num2str(tag_angle(1)));
    tag_angle = tag_angle(1).*ones(1, Run_Count);
else
    tag_angle = linspace(tag_angle(1), tag_angle(2), 1000);
    dim = dim + 1;
end

if ~(strcmp(data_vis{1}, 'LO_Noise_1') | strcmp(data_vis{1}, 'LO_Noise'))
    set_param('LL_rdf2/LO', 'noise_i', num2str(LO_Noise_1(1)));
    LO_Noise_1 = LO_Noise_1(1).*ones(1, Run_Count);
else
    LO_Noise_1 = linspace(LO_Noise_1(1), LO_Noise_1(2), 1000);
    dim = dim + 1;
end

if ~(strcmp(data_vis{1}, 'LO_Noise_2') | strcmp(data_vis{1}, 'LO_Noise'))
    set_param('LL_rdf2/LO', 'noise_q', num2str(LO_Noise_2(1)));
    LO_Noise_2 = LO_Noise_2(1).*ones(1, Run_Count);
else
    LO_Noise_2 = linspace(LO_Noise_2(1), LO_Noise_2(2), 1000);
    dim = dim + 1;
end

if ~(strcmp(data_vis{1}, 'RF_Noise_1') | strcmp(data_vis{1}, 'RF_Noise'))
    set_param('LL_rdf2/Tag', 'np_1', num2str(RF_Noise_1(1)));
    RF_Noise_1 = RF_Noise_1(1).*ones(1, Run_Count);
else
    RF_Noise_1 = linspace(RF_Noise_1(1), RF_Noise_1(2), 1000);
    dim = dim + 1;
end

if ~(strcmp(data_vis{1}, 'RF_Noise_2') | strcmp(data_vis{1}, 'RF_Noise'))
    set_param('LL_rdf2/Tag', 'np_2', num2str(RF_Noise_2(1)));
    RF_Noise_2 = RF_Noise_2(1).*ones(1, Run_Count);
else
    RF_Noise_2 = linspace(RF_Noise_2(1), RF_Noise_2(2), 1000);
    dim = dim + 1;
end

if ~(strcmp(data_vis{1}, 'Separation'))
    set_param('LL_rdf2/Tag', 'sep', num2str(TOI_ANT_Separation(1)));
    TOI_ANT_Separation = TOI_ANT_Separation(1).*ones(1, Run_Count);
else
    TOI_ANT_Separation = linspace(TOI_ANT_Separation(1), TOI_ANT_Separation(2), 1000);
    dim = dim + 1;
end

% dim = length(data_vis);
% if dim > 3 
%     error('Please keep it 2D or 3D, thats absurd')
% elseif dim == 3
% end

% param = {};
% for p = 1:dim
%     param{p} = paramMap(data_vis(p));
% end

% for i = 1:Run_Count
%     set_param('LL_rdf2/Tag', 'np_1', num2str(RF_Noise_1(i)));
%     set_param('LL_rdf2/Tag', 'np_2', num2str(RF_Noise_2(i)));
% 


figure
xlabel('Time (s)')
ylabel('Bearing (degrees)')
title('LO Noise and effect on bearing over a time span')
hold on
for j = 1:(1000/Run_Count):1000
    if ~(strcmp(data_vis{1}, 'tag_angle') | strcmp(data_vis{1}, 'TOI_ANT_Separation'))
        set_param(path{1}, path{2}, path{3});
        set_param(path{4}, path{5}, path{6});   
        sim('LL_rdf2')
        plot(data_out.time, data_out.signals.values)
    else
        set_param(path{1}, path{2}, path{3});
        sim('LL_rdf2')
        plot(data_out.time, data_out.signals.values)
    end
    
end
    
% Visualize Data
% 
% if strcmp(data_vis(1), 'time')
%     xvar = 'time (s)';
%     if strcmp(data_vis(2), 'Bearing')
%         yvar = 'Bearing (deg)';
%         plot(tout(1,:), yout)
%     end
% elseif strcmp(data_vis(1), 'LO_Noise')
%     xvar = 'LO Noise Level (?)';
%     if strcmp(data_vis(2), 'Bearing')
%         yvar = 'Bearing (deg)';
%         hold on
%         for n = 1:Run_Count
%             plot(LO_Noise_1, yout(n,:))
%         end
%     end
% elseif strcmp(data_vis(1), 'RF_Noise')
%     xvar = 'RF Noise Level (?)';
%     if strcmp(data_vis(2), 'Bearing')
%         yvar = 'Bearing (deg)';
%         plot(RF_Noise_1, yout)
%     end
% end
% 
% 
% % Format Plot Tools
% z = cell(1, Run_Count);
% for L = 1:Run_Count
%     z{L} = sprintf('An: %.3f , Sp: %.3f', tag_angle(L), TOI_ANT_Separation(L));
% end
% legend(z(:));
% xlabel(xvar)
% ylabel(yvar)
            
