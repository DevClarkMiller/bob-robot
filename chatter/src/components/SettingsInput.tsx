import React, { useMemo } from 'react';
import { Card, CardContent } from './ui/card';
import { Label } from './ui/label';
import { Input } from './ui/input';
import { Switch } from './ui/switch';

export interface SettingsInputProps {
	unitGuid: string;
	isUsingAIChats: boolean;
	setUnitGuid: React.Dispatch<React.SetStateAction<string>>;
	onChangeIsUsingAIChats: (state: boolean) => Promise<void>;
}

const SettingsInput = ({ unitGuid, isUsingAIChats, setUnitGuid, onChangeIsUsingAIChats }: SettingsInputProps) => {
	const aiToggleEnabled = useMemo(() => !!unitGuid, [unitGuid]);

	return (
		<Card>
			<CardContent>
				<>
					<Label htmlFor="unitGuid" className="mb-2">
						Unit GUID
					</Label>
					<Input
						name="unitGuid"
						placeholder="87487969-0636-4196-9e1a-718f6c80e626"
						value={unitGuid}
						onChange={e => setUnitGuid(e.target.value)}
					/>
				</>
				<div className="my-2 flex gap-3">
					<Label htmlFor="isUsingAIChats">AI Chats</Label>
					<Switch
						disabled={!aiToggleEnabled}
						id="isUsingAIChats"
						name="isUsingAIChats"
						checked={isUsingAIChats}
						onCheckedChange={onChangeIsUsingAIChats}
						aria-label="Toggle AI Chats"
					/>
				</div>
			</CardContent>
		</Card>
	);
};

export default SettingsInput;
